#include "quantik.hpp"

#include "State.hpp"

// TODO: Move to quantik.hpp
#include <fstream>
#define MAX_DEPTH 16 // TODO: 16

bool save_current_level(state_map& current_level, char depth)
{
    string file_path = "level" + STR(depth) + ".qtk";

    // Open the file in binary mode
    std::ofstream out_file(file_path, std::ios::out | std::ios::binary);
    if (! out_file.is_open()) return false;

    for (auto& pair : current_level)
    {
        out_file.write((const char*) &pair.first, sizeof(encoding));
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

    DBGMSG(DBG_COMPUTE_GAMETREE, time_info);

    DBGMSG(true, STR(total_nodes) + " Total Nodes | ");
    DBGMSG(true, STR(current_level.size()) + " Leaf Nodes\n");
}

int main()
{
    state_map current_level{};
    state_map next_level;
    int total_nodes = 1;

    State root_state{};
    current_level.emplace(root_state.encode(), root_state);

    auto tn = std::chrono::steady_clock::now();

    DBGMSG(DBG_COMPUTE_GAMETREE, "Beginning to compute the tree...\n");
    DBGMSG(DBG_COMPUTE_GAMETREE, "Level 0 ... ");
    DBGMSG(DBG_COMPUTE_GAMETREE, STR(0) + " Total Nodes | ");
    DBGMSG(DBG_COMPUTE_GAMETREE, STR(1) + " Leaf Nodes\n");

    for (char depth = 1; depth <= MAX_DEPTH; depth++)
    {
        DBGMSG(true, "Level " + STR(depth) + " ... ");
        cout << std::flush;

        for (auto& pair : current_level)
        {
            
            int piece_count = 0;
            for (auto& smth : pair.second.placed_pieces) piece_count += smth.second.size();
            assert(piece_count == depth - 1);

            state_map children = pair.second.compute_following_states();
            next_level.insert(children.begin(), children.end());
        }
        
        if (!save_current_level(current_level, depth - 1)) return -1;
        total_nodes += next_level.size();

        prints(next_level, total_nodes, tn);

        current_level = next_level;
        next_level.clear();
    }

    return 0;
}
