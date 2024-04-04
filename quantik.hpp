#pragma once

// --- Standarad Library Includes ---

#include <cctype>
#include <cstring>
#include <cassert>
#include <unordered_map>
#include <map>
#include <iostream>
#include <algorithm>

// --- Namespace Useages ---
using 
    std::string, 
    std::cout,
    std::endl,
    std::map,
    std::unordered_map,
    std::vector;

// --- Forward Declarations ---

class State;
class GameTreeNode;

// --- Enums ---

enum class SwapType { ROWS, COLUMNS };
enum class PieceType {
    EMPTY, 
    WHITE_SQUARE, 
    BLACK_SQUARE, 
    WHITE_TRIANGLE, 
    BLACK_TRIANGLE, 
    WHITE_CIRCLE, 
    BLACK_CIRCLE, 
    WHITE_DIAMOND, 
    BLACK_DIAMOND
};

// --- Typedefs ---

typedef uint64_t encoding;
typedef unordered_map<encoding, State> state_map;
typedef unordered_map<PieceType, vector<std::pair<char, char>>> pieces_map;
typedef unordered_map<encoding, GameTreeNode*> node_ptr_map;