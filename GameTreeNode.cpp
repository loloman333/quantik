#include "GameTreeNode.hpp"

GameTreeNode::GameTreeNode(State state)
{
    this->state = state;
}

string GameTreeNode::get_print_string()
{
    return this->state.get_print_string();
}

node_ptr_map &GameTreeNode::get_children()
{
    return this->children;
}

node_ptr_map &GameTreeNode::generate_children(node_ptr_map &all_nodes)
{
    state_map following_states{this->state.compute_following_states()};

    for (auto& pair : following_states)
    {
        auto node = all_nodes.find(pair.first);

        if (node != all_nodes.end())
        {
            this->children.emplace(pair.first, node->second);
        }
        else
        {
            GameTreeNode *new_node = new GameTreeNode(pair.second);
            this->children.emplace(pair.first, new_node);
            all_nodes.emplace(pair.first, new_node);
        }
    }

    return this->children;
}