#include "GameTree.hpp"


string get_node_ptr_map_str(node_ptr_map& map)
{
    std::stringstream ss;
    for (auto& pair : map)
    {
        ss << "Encoding: " << pair.first << endl;
        ss << "State " << pair.second->get_print_string() << endl;
    }

    return ss.str();
}

GameTree::GameTree(State root_state)
{
    GameTreeNode* root_node = new GameTreeNode(root_state);
    this->root = root_node;
    this->all_nodes.emplace(root_state.encode(), this->root);
    this->leaf_nodes.emplace(root_state.encode(), this->root);
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
    for (auto& pair : this->leaf_nodes) assert(pair.second->get_children().size() == 0);
    for (auto& pair : this->leaf_nodes)
    {
        node_ptr_map& children = pair.second->generate_children(this->all_nodes);
        for (auto& child : children)
        {
            assert(child.first != 0);
            new_leaf_nodes.insert(child);
        }
    }

    this->leaf_nodes = new_leaf_nodes;
}

string GameTree::get_print_string()
{
    string s = "";
    s += STR(all_nodes.size()) + " Total Nodes | ";
    s += STR(leaf_nodes.size()) + " Leaf Nodes\n";
    s += "All leaf nodes: \n";
    s+= get_node_ptr_map_str(leaf_nodes);
    return s;
}

GameTree* GameTree::compute_tree(char depth, State root_state)
{
    GameTree* tree = new GameTree{root_state};

    auto t0 = std::chrono::steady_clock::now();

    DBGMSG(DBG_COMPUTE_GAMETREE, "Beginning to compute the tree...\n");
    DBGMSG(DBG_COMPUTE_GAMETREE, "Level 0 ...\n");
    DBGMSG(DBG_COMPUTE_GAMETREE, STR(tree->all_nodes.size()) + " Total Nodes | ");
    DBGMSG(DBG_COMPUTE_GAMETREE, STR(tree->leaf_nodes.size()) + " Leaf Nodes\n");
    DBGMSG(DBG_COMPUTE_GAMETREE_DETAILED, "All leaf nodes: \n" + get_node_ptr_map_str(tree->leaf_nodes));

    for (int i = 1; i <= depth; i++)
    {
        DBGMSG(DBG_COMPUTE_GAMETREE, "Level " + STR(i) + " ... ");

        tree->compute_next_layer();
        auto tn = std::chrono::steady_clock::now();

        std::chrono::duration<double> duration = tn- t0;
        
        DBGMSG(DBG_COMPUTE_GAMETREE, STR(duration.count()) + " seconds \n" + STR(tree->all_nodes.size()) + " Total Nodes | ");
        DBGMSG(DBG_COMPUTE_GAMETREE, STR(tree->leaf_nodes.size()) + " Leaf Nodes\n");
        DBGMSG(DBG_COMPUTE_GAMETREE_DETAILED, "All leaf nodes: \n" + get_node_ptr_map_str(tree->leaf_nodes));
    }
    
    return tree;
}