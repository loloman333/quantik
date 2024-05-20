#include "quantik.hpp"

#include "State.hpp"
#include "TimeManager.hpp"
#include "FileManager.hpp"

bool generate_states(char max_depth)
{
    state_map current_level{};
    state_map next_level;
    int total_nodes = 0;

    DBGMSG(DBG_COMPUTE_STATES, "Beginning to generate states...\n");

    State root_state{};
    current_level.emplace(root_state.encode(), root_state);

    TimeManager::start();

    for (char depth = 0; depth <= max_depth; depth++)
    {
        DBGMSG(DBG_COMPUTE_STATES, "Level " + STR(depth) + " ... ");

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

        if (! FileManager::save_level_tmp(current_level, depth)) return false;
        total_nodes += current_level.size();

        string time_info = TimeManager::to_string(TimeManager::step()) + " | ";
        DBGMSG(DBG_COMPUTE_STATES, time_info);
        DBGMSG(DBG_COMPUTE_STATES, STR(total_nodes) + " Total States | ");
        DBGMSG(DBG_COMPUTE_STATES, STR(current_level.size()) + " Leaf States\n");

        if (depth == max_depth) assert(next_level.size() == 0);
        current_level = next_level;
        next_level.clear();
    }

    return true;
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

// win_code compute_code(map<encoding, win_code>& children)
// {
//     win_code best_child_code = UNDEFINED_CODE;
//     for (auto& child : children) 
//     {
//         win_code child_code = child.second;
//         assert(child_code != UNDEFINED_CODE);

//         // Still undefined
//         if (best_child_code == UNDEFINED_CODE) 
//         {
//             best_child_code = child_code;
//         }
//         // Draw code
//         else if (child_code == DRAW_CODE)
//         { 
//             if (best_child_code % 2 == 0) best_child_code = DRAW_CODE;
//         }
//         // 
//         else if (child_code % 2 == 0)
//         {
//             if (best_child_code % 2 == 1) best_child_code = child_code;
//             else if (child_code > best_child_code) best_child_code = child_code;
//         }
//         // 
//         else
//         { 
//             if (best_child_code % 2 == 1 && child_code < best_child_code) best_child_code = child_code;
//         }
//     }

//     assert(best_child_code != UNDEFINED_CODE); 
//     return (best_child_code == DRAW_CODE) ? DRAW_CODE : best_child_code + 1;
// }

bool generate_codes(char max_depth)
{
    DBGMSG(DBG_COMPUTE_CODES, "Beginning to compute codes...\n");

    map<encoding, win_code> current_level;
    map<encoding, win_code> last_level;

    TimeManager::start(true);

    for (char depth = max_depth; depth >= 0; depth--)
    {
        DBGMSG(DBG_COMPUTE_CODES, "Level " + STR(depth) + " ... ");
        cout << std::flush;

        if (! FileManager::read_level(current_level, depth)) return false;

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

        FileManager::save_level_final(current_level, depth);

        string time_info = TimeManager::to_string(TimeManager::step()) + " | ";
        DBGMSG(DBG_COMPUTE_CODES, time_info);
        DBGMSG(DBG_COMPUTE_CODES, STR(current_level.size()) + " states\n");

        last_level = current_level;
        current_level.clear();
    }

    return true;
}

int main()
{
    char depth = 16;

    FileManager::delete_all_files();

    if (! generate_states(depth)) return -1;

    if (! generate_codes(depth)) return -1;
}
