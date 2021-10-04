#include "tree/binary_tree.hpp"
#include "heap/heap.hpp"
#include "tree/binary_search_tree.hpp"
#include "delist/delist.hpp"
#include "tree/rbtree.hpp"

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

void test_binary_search_tree(){
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
}

void test_delist(){
    algorithm::delist::delist<int> l1, l2;

    for(int i=0; i<10; i++){
        l1.emplace_back(i);
        // l2.emplace(i);
        // if(i % 3 == 0){
        //     // l2.pop();
        //     l1.pop();
        // }
    }

    l1.tranverse();
    // l1.reverse_tranverse();
    // l2.tranverse();
    // l2.reverse_tranverse();
}

void test_rb_tree(){
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 1000);
    algorithm::tree::rb_tree<int> rbTree;

    for(int i=0; i<10; i++){
        int num = u(e) % 30;
        std::cout << num << std::endl;
        rbTree.insert(num);
    }
    std::cout << std::endl;

    // auto seq = rbTree.inOrder();

    // for(auto&& num : seq){
    //     std::cout << num << ' ';
    // }
    // std::cout << std::endl;
}

int main(){
    // test_binary_tree();
    // test_heap();
    // test_binary_search_tree();
    // test_delist();
    test_rb_tree();

    return 0;
}