#include "tree/binary_tree.hpp"
#include "heap/heap.hpp"
#include "tree/binary_search_tree.hpp"

void test_binary_tree(){
    vector<int> seq;
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 1000);

    for(int i=0; i<1000; i++){
        seq.emplace_back(u(e));
    }

    algorithm::tree::tree<int> bt(seq);

    auto level = bt.levelOrder();
    auto pre = bt.preOrder();
    auto in = bt.inOrder();
    auto post = bt.postOrder();
    for(auto&& num : level){
        std::cout << num << ' ';
    }
    std::cout << std::endl;
    for(auto&& num : pre){
        std::cout << num << ' ';
    }
    std::cout << std::endl;
    for(auto&& num : in){
        std::cout << num << ' ';
    }
    std::cout << std::endl;
    for(auto&& num : post){
        std::cout << num << ' ';
    }
    std::cout << std::endl;
}

void test_heap(){
    vector<int> seq;
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 1000);

    algorithm::heap::heap<int> h;

    for(int i=0; i<1000; i++){
        h.push(u(e));
    }

    while(!h.empty()){
        seq.push_back(h.pop());
    }

    for(auto&& num : seq){
        std::cout << num << ' ';
    }
    std::cout << std::endl;
}


int main(){
    // test_binary_tree();
    // test_heap();
    algorithm::tree::bs_tree<int> bst;

    vector<int> seq;
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 1000);

    for(int i=0; i<100; i++){
        bst.insert(u(e));
    }

    auto in = bst.inOrder();

    for(auto&& num : in){
        std::cout << num << ' ';
    }
    std::cout << std::endl;

    return 0;
}