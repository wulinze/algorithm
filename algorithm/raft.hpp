#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using std::shared_ptr;
using std::vector;
using std::string;
using std::cout;
using std::endl;

namespace algorithm::distribute{
    enum class Identity{
        LEADER, FOLLOWER, CANDIDATE 
    };
    struct NodeState{
        Identity node_identity;


    };

    class Client{
    private:
        
    public:

    };

    class Server{
    private:
        NodeState node_state;

    public:
    };
}