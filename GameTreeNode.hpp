#pragma once

#include "quantik.hpp"
#include "State.hpp"

class GameTreeNode
{
    // -------------
    // Private Stuff
    // -------------
    private: 

    // --- Members ---

    State state{};
    node_ptr_map children{};

    // --- Methods ---

    // -------------
    // Public Stuff
    // -------------
    public: 

    // --- Constructors ---

    GameTreeNode(State state);    

    // --- Methods ---

    node_ptr_map& get_children();

    node_ptr_map& generate_children(node_ptr_map& all_nodes);

    // -------------
    // Static Stuff
    // -------------

};