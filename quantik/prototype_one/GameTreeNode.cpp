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
    if (this->state.is_winning_state())
    {
        this->code = LOSE_CODE;
        return this->children;
    }

    state_map following_states{this->state.compute_following_states()};
    if (following_states.size() == 0)
    {
        this->code = DRAW_CODE;
    }

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

node_code GameTreeNode::compute_code()
{
    if (this->code != UNDEFINED_CODE) return this->code;

    node_code best_code = UNDEFINED_CODE;
    for (auto& child : this->children) 
    {
        node_code child_code = child.second->compute_code();

        // Still undefined
        if (best_code == UNDEFINED_CODE) 
        {
            best_code = child_code;
            continue;
        }
        // Win code
        else if (child_code % 2 == 0)
        {
            if (best_code == DRAW_CODE) best_code = child_code;
            if (best_code % 2 == 1) best_code = child_code;
            else if (child_code < best_code) best_code = child_code;
        }
        // Draw code
        else if (child_code == DRAW_CODE)
        { 
            if (best_code % 2 == 1) best_code = child_code;
        }
        // Lose code
        else
        { 
            if (child_code > best_code) best_code = child_code;
        }
    }

    assert(best_code != UNDEFINED_CODE);
    this->code = (best_code == DRAW_CODE) ? DRAW_CODE : best_code + 1; 
    return this->code;
}