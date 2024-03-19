#include "definitions.hpp"
#include "state.cpp"

typedef unordered_map<encoding, GameTreeNode*> node_ptr_map;

struct GameTree
{
    GameTree()
    {
        State empty_state{};
        GameTreeNode* root_node = new GameTreeNode(empty_state);
        this->root = root_node;
        this->all_nodes.emplace(empty_state.encode_base_9(), this->root);
        this->leaf_nodes.emplace(empty_state.encode_base_9(), this->root);
    }

    // Members

    GameTreeNode* root;
    node_ptr_map all_nodes{};
    node_ptr_map leaf_nodes{};

    // Methods

    void compute_next_layer()
    {  
        node_ptr_map new_leaf_nodes{};
        for (auto pair : this->leaf_nodes)
        {
            
        }
    }
};

struct GameTreeNode 
{
    GameTreeNode() = delete;
    GameTreeNode(State state)
    {
        this->state = state;
    }   
    
    // Members

    State state;
    node_ptr_map children{};

    // Methods
};