#include "quantik.hpp"

#include "State.hpp"

// TODO: Move to quantik.hpp
#include <fstream>
#define MAX_DEPTH 16 // TODO: 16

bool save_current_level(state_map& current_level, char depth)
{
    string file_path = "_level" + STR(depth) + ".qtk";
    std::ofstream out_file(file_path, std::ios::out | std::ios::binary);
    if (! out_file.is_open()) return false;

    for (auto& pair : current_level)
    {
        out_file.write((const char*) &pair.first, sizeof(encoding));
        out_file.write((const char*) &pair.second.code, sizeof(win_code));
    }

    out_file.close();
    return out_file.good();
}

bool save_current_level(map<encoding, win_code>& current_level, char depth)
{
    string file_path = "level" + STR(depth) + ".qtk";
    std::ofstream out_file(file_path, std::ios::out | std::ios::binary);
    if (! out_file.is_open()) return false;

    for (auto& pair : current_level)
    {
        out_file.write((const char*) &pair.first, sizeof(encoding));
        out_file.write((const char*) &pair.second, sizeof(win_code));
    }

    out_file.close();
    return out_file.good();
}

void prints(state_map& current_level, int total_nodes, std::chrono::_V2::steady_clock::time_point& tn)
{
    auto tn_plus_1 = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = tn_plus_1 - tn;
    tn = tn_plus_1;
    int minutes = static_cast<int>(duration.count()) / 60;
    double seconds = duration.count() - minutes * 60;
    string minute_info = minutes != 0 ? STR(minutes) + " minutes " : "";
    string time_info = minute_info + STR(seconds) + " seconds | ";

    DBGMSG(DBG_COMPUTE_STATES, time_info);

    DBGMSG(DBG_COMPUTE_STATES, STR(total_nodes) + " Total Nodes | ");
    DBGMSG(DBG_COMPUTE_STATES, STR(current_level.size()) + " Leaf Nodes\n");
}

bool generate_states(char max_depth)
{
    state_map current_level{};
    state_map next_level;
    int total_nodes = 0;

    DBGMSG(DBG_COMPUTE_STATES, "Beginning to generate states...\n");

    State root_state{};
    current_level.emplace(root_state.encode(), root_state);

    auto tn = std::chrono::steady_clock::now();

    for (char depth = 0; depth <= max_depth; depth++)
    {
        DBGMSG(true, "Level " + STR(depth) + " ... ");
        cout << std::flush;

        for (auto& pair : current_level)
        {
            if (pair.second.is_winning_state())
            {
                pair.second.code = LOSE_CODE;
                continue;
            }

            if (depth == max_depth && max_depth != MAX_DEPTH)
            {
                pair.second.code = DRAW_CODE;
                continue;
            }

            state_map children = pair.second.compute_following_states();
            if (children.size() == 0) pair.second.code = DRAW_CODE;
            next_level.insert(children.begin(), children.end());
        }

        if (!save_current_level(current_level, depth)) return false;
        total_nodes += current_level.size();
        prints(current_level, total_nodes, tn);

        if (depth == max_depth) assert(next_level.size() == 0);
        current_level = next_level;
        next_level.clear();
    }

    return true;
}

bool read_level(char depth, map<encoding, win_code>& level)
{
    string filename = "_level" + STR(depth) + ".qtk";
    std::ifstream file(filename, std::ios::binary);

    if (! file.is_open()) return false;

    while (!file.eof())
    {
        encoding enc;
        win_code code;

        if (! file.read((char*) &enc, sizeof(encoding))) 
        {
            if (file.eof()) return true;
            DBGMSG(DBG_COMPUTE_CODES, "Some read error...\n");
            return false;
        }

        if (! file.read((char*) &code, sizeof(win_code)))
        {
            DBGMSG(DBG_COMPUTE_CODES, "Some read error 2...\n");
            return false;
        }

        level.emplace(enc, code);

        if (file.fail())
        {
            DBGMSG(DBG_COMPUTE_CODES, "Some read error 3...\n");
            return false;
        }
    }

    file.close();
    return file.good();
}

win_code compute_code(map<encoding, win_code>& children)
{
    win_code best_child_code = UNDEFINED_CODE;
    for (auto& child : children) 
    {
        win_code child_code = child.second;
        assert(child_code != UNDEFINED_CODE);

        // Still undefined
        if (best_child_code == UNDEFINED_CODE) 
        {
            best_child_code = child_code;
        }
        // Draw code
        else if (child_code == DRAW_CODE)
        { 
            if (best_child_code % 2 == 1) best_child_code = DRAW_CODE;
        }
        // Win code
        else if (child_code % 2 == 0)
        {
            if (best_child_code == DRAW_CODE || best_child_code % 2 == 1) best_child_code = child_code;
            else if (child_code < best_child_code) best_child_code = child_code;
        }
        // Lose code
        else
        { 
            if (best_child_code % 2 == 1 && child_code > best_child_code) best_child_code = child_code;
        }
    }

    assert(best_child_code != UNDEFINED_CODE); 
    return (best_child_code == DRAW_CODE) ? DRAW_CODE : best_child_code + 1;
}

bool generate_codes(char max_depth)
{
    DBGMSG(DBG_COMPUTE_CODES, "Beginning to compute codes...\n");

    map<encoding, win_code> current_level;
    map<encoding, win_code> last_level;

    for (char depth = max_depth; depth >= 0; depth--)
    {
        DBGMSG(DBG_COMPUTE_CODES, "Level " + STR(depth) + " ... ");

        if (!read_level(depth, current_level)) return false;
        DBGMSG(DBG_COMPUTE_CODES, "read!\n");

        for (auto& pair : current_level)
        {
            if (pair.second == UNDEFINED_CODE)
            {
                assert(depth != max_depth);

                State state = State::decode(pair.first);
                
                map<encoding, win_code> children; 
                
                for (auto& child : state.compute_following_states())
                {
                    auto it = last_level.find(child.first);
                    assert(it != last_level.end());
                    children.emplace(child.first, it->second);
                }

                pair.second = compute_code(children);
            }
        }

        save_current_level(current_level, depth);

        last_level = current_level;
        current_level.clear();
    }

    return true;
}

int main()
{
    char depth = 7;

    for (int i = 0; i <= MAX_DEPTH; i++)
    {
        string file1 = ("level" + STR(i) + ".qtk"); 
        string file2 = ("_level" + STR(i) + ".qtk"); 
        std::remove(file1.c_str());
        std::remove(file2.c_str());
    }

    if (! generate_states(depth)) return -1;

    if (! generate_codes(depth)) return -1;
}
