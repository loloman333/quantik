#pragma once

#include "State.hpp"
#include "quantik.hpp"

class TransformationGenerator
{
    public: 

    // -------------
    // Static Stuff
    // -------------

    static string generate_mirror_function();
    static string generate_rotate_90_function();
    static string generate_rotate_180_function();
    static string generate_rotate_270_function();
    static string generate_swap_rows_or_cols_function(SwapType type, int index1, int index2, bool two_swaps = false, int second_index1 = 0, int second_index2 = 0);

};