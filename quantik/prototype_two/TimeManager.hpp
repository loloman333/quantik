#pragma once

#include "quantik.hpp"

class TimeManager
{

    // -------------
    // Private Stuff
    // -------------
    private: 

    // --- Members ---

    static std::chrono::steady_clock::time_point tn;
    static std::chrono::duration<double> total_time;

    // -------------
    // Public Stuff
    // -------------
    public: 

    // --- Methods ---

    static void start(bool keep_total = false);
    static std::chrono::duration<double> step();
    static std::chrono::duration<double> get_total_time();
    static string to_string(std::chrono::duration<double> duration, string seperator = "");

};