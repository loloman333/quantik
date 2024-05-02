#include "FileManager.hpp"

bool FileManager::save_level_tmp(state_map& current_level, char depth)
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

bool FileManager::save_level_final(map<encoding, win_code>& current_level, char depth)
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

bool FileManager::read_level(map<encoding, win_code>& level, char depth)
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
