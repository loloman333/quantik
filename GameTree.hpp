#pragma once

#include "quantik.hpp"
#include "State.hpp"
#include "GameTreeNode.hpp"

class GameTree 
{
    friend int main(); //TODO: remove?

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

    GameTree(State root_state = State{});
    GameTree(const GameTree& other) = delete;
    ~GameTree();

    // --- Methods ---

    void compute_next_layer();
    string get_print_string();

    // -------------
    // Static Stuff
    // -------------

    static GameTree* compute_tree(char depth, State root_state = State{});
};