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

    // TODO: let "compute_following_states" return map instead already with canonicals ???
    vector<State> canonical_states{};
    for (State &state : this->state.compute_following_states())
    {
        canonical_states.push_back(state.compute_canonical());
    }

    for (State &canonical : canonical_states)
    {
        encoding code = canonical.encode();
        auto node = all_nodes.find(code);

        if (node != all_nodes.end())
        {
            children.emplace(code, node->second);
        }
        else
        {
            GameTreeNode *new_node = new GameTreeNode(canonical);
            children.emplace(code, new_node);
            all_nodes.emplace(code, new_node);
        }
    }

    return this->children;
}