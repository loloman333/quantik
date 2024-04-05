#include "GameTree.hpp"

GameTree::GameTree()
{
    State empty_state{};
    GameTreeNode* root_node = new GameTreeNode(empty_state);
    this->root = root_node;
    this->all_nodes.emplace(empty_state.encode(), this->root);
    this->leaf_nodes.emplace(empty_state.encode(), this->root);
}

GameTree::~GameTree()
{
    for (auto& pair : this->all_nodes)
    {
        delete pair.second;
    }
}

void GameTree::compute_next_layer()
{  
    node_ptr_map new_leaf_nodes{};
    for (auto& pair : this->leaf_nodes)
    {
        node_ptr_map& children = pair.second->get_children();
        // assert(children.size() == 0); // TODO: is this condition always true?
        pair.second->generate_children(this->all_nodes);
        for (auto& child : children)
        {
            new_leaf_nodes.insert(child);
        }
    }

    this->leaf_nodes = new_leaf_nodes;
}

GameTree GameTree::compute_tree(char depth)
{
    GameTree tree{};

    DBGMSG(DBG_COMPUTE_GAMETREE, "Beginning to compute the tree...\n");
    DBGMSG(DBG_COMPUTE_GAMETREE, "Level 0 ... ");
    DBGMSG(DBG_COMPUTE_GAMETREE, STR(tree.all_nodes.size()) + " Total Nodes | ");
    DBGMSG(DBG_COMPUTE_GAMETREE, STR(tree.leaf_nodes.size()) + " Leaf Nodes\n");

    for (int i = 1; i <= depth; i++)
    {
        cout << "Layer " << i << " ... ";
        DBGMSG(DBG_COMPUTE_GAMETREE, "Level " + STR(i) + " ... ");

        tree.compute_next_layer();
        
        DBGMSG(DBG_COMPUTE_GAMETREE, STR(tree.all_nodes.size()) + " Total Nodes | ");
        DBGMSG(DBG_COMPUTE_GAMETREE, STR(tree.leaf_nodes.size()) + " Leaf Nodes\n");
    }
    
    return tree;
}