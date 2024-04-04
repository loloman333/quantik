#pragma once

#include "quantik.hpp"
#include "State.hpp"
#include "GameTreeNode.hpp"

class GameTree 
{

    // -------------
    // Private Stuff
    // -------------
    private: 

    // --- Members ---

    GameTreeNode* root{};
    node_ptr_map all_nodes{};
    node_ptr_map leaf_nodes{};

    // --- Methods ---

    // -------------
    // Public Stuff
    // -------------
    public: 

    // --- Constructors ---

    GameTree();
    ~GameTree();

    // --- Methods ---

    void compute_next_layer();

    // -------------
    // Static Stuff
    // -------------

    static GameTree compute_tree(char depth);
};