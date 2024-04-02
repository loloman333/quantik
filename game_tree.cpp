#include "definitions.hpp"
#include "state.cpp"
#include "canonical_generator.cpp"

struct GameTreeNode;

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

    ~GameTree()
    {
        for (auto& pair : this->all_nodes)
        {
            delete pair.second;
        }
    }

    // Members

    GameTreeNode* root;
    node_ptr_map all_nodes{};
    node_ptr_map leaf_nodes{};

    // Methods

    void compute_next_layer()
    {  
        node_ptr_map new_leaf_nodes{};
        for (auto& pair : this->leaf_nodes)
        {
            assert(pair.second->children.size() == 0); // TODO: is this condition always true?
            pair.second->generate_children(this->all_nodes);
            for (auto& child : pair.second->children)
            {
                new_leaf_nodes.insert(child);
            }
        }

        this->leaf_nodes = new_leaf_nodes;
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

    node_ptr_map& generate_children(node_ptr_map& all_nodes)
    {
        // TODO: generate all possible moves and create new state for each
        // TODO: why do I need ref here???
        vector<State> following_states{compute_following_states(this->state)};

        // TODO: convert new state to canonical
        vector<State> canonical_states{};
        for (State& state : following_states)
        {
            canonical_states.push_back(compute_canonical(state));
        }

        // TODO: check if canonical already in existing nodes; if no: add to all_nodes; else: just add reference
        for (State& canonical : canonical_states)
        {
            encoding code = canonical.encode_base_9();
            auto node = all_nodes.find(code);

            if (node != all_nodes.end())
            {
                children.emplace(code, node->second);
            }
            else 
            {
                GameTreeNode* new_node = new GameTreeNode(canonical);
                children.emplace(code, new_node);
                all_nodes.emplace(code, new_node);
            }
            
        }
        
        return this->children;
    }
};