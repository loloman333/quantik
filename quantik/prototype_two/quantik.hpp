#pragma once

// --- Standarad Library Includes ---

#include <cassert>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <set>
#include <map>
#include <chrono>
#include <fstream>

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
enum class PieceShape {
    NONE,
    SQUARE,
    TRIANGLE,
    CIRCLE,
    DIAMOND
};

// --- Typedefs ---

typedef uint64_t encoding;
typedef unsigned char win_code;
typedef unordered_map<encoding, State> state_map;
typedef unordered_map<PieceType, vector<std::pair<char, char>>> pieces_map;

// --- Defines ---

#define MAX_DEPTH 16

#define DRAW_CODE 20
#define LOSE_CODE 0
#define UNDEFINED_CODE 21

#define STR(x) std::to_string(x)
#define DBGMSG(var, msg) if (var) cout << msg

#define DBG_CANONICAL_GENERATOR false
#define DBG_CANONICAL_GENERATOR_DETAILED false
#define DBG_COMPUTE_STATES true
#define DBG_COMPUTE_CODES true
