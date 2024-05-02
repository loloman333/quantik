#pragma once

#include "quantik.hpp"

class FileManager
{

    // -------------
    // Private Stuff
    // -------------
    private: 

    // --- Members ---

    // --- Methods ---


    // -------------
    // Public Stuff
    // -------------
    public: 

    // --- Methods ---

    static bool save_level_tmp(state_map& current_level, char depth);
    static bool save_level_final(map<encoding, win_code>& current_level, char depth);
    static bool read_level(map<encoding, win_code>& level, char depth);
};