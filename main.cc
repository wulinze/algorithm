#pragma once

#include "tree/binary_tree.hpp"
#include "heap/heap.hpp"
#include "tree/binary_search_tree.hpp"
#include "delist/delist.hpp"
#include "tree/rbtree.hpp"
#include "tree/btree.hpp"
#include "tree/trie.hpp"
#include "algorithm/math.hpp"
#include "ptr/shared_ptr.hpp"
#include <unordered_set>

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
    std::default_random_engine e(time(0));
    std::uniform_int_distribution<int> u(0, 1000);
    algorithm::tree::rb_tree<int> rbTree;

    for(int i=0; i<100; i++){
        int num = u(e) % 30;
        std::cout << num << ' ';
        rbTree.insert(num);
    }
    std::cout << std::endl;

    auto seq = rbTree.inOrder();

    for(auto&& num : seq){
        if(num != -1)
            std::cout << num << ' ';
    }
    std::cout << std::endl;
}

void test_b_tree(){
    using namespace algorithm::tree;
    b_tree<int, int> tree(7);
    const int tree_cap = 20;
    srand(time(0));
    // insert test
    std::cout << "insert testing" << std::endl;
    for(int i=0; i<tree_cap; i++){
        int num =  rand() % 100;
        tree.insert(i, num);
    }
    std::cout << "insert test pass" << std::endl;
    
    // tranverse test
    std::cout << "tranverse testing" << endl;
    tree.level_tranverse();
    std::cout << "tranverse test pass" << endl;

    std::cout << "find testing" << endl;
    const int times = 10;
    for(int i=1; i<=times; i++){
        int key = rand() % tree_cap;
        cout << "keys: "  << key << ", vals:" << tree.find(key) << endl;
    }
    std::cout << "find test pass" << endl;

    // remove test
    const int cap = 30;
    using std::unordered_set;
    unordered_set<int> del(cap);
    for(int i=0; i<cap; i++){
        del.insert(rand() % tree_cap);
    }
    // for(auto&& key : del){
    //     cout << key << endl;
    //     tree.remove(key);
    //     cout << key << " remove success" << endl;
    //     tree.level_tranverse();
    // }
    tree.remove(7);
    cout << 7 << " remove success" << endl;
    tree.level_tranverse();
    // tree.level_tranverse();
}

void test_trie(){
    vector<string> seq;
    std::default_random_engine e(time(0));
    std::uniform_int_distribution<int> u(0, 25);
    algorithm::tree::trie t;

    for(int i=0; i<5; i++){
        string word;

        for(int j=0; j<u(e)+1; j++){
            word.push_back('a' + u(e));
        }
        cout << word << endl;
        t.insert(word);
        seq.push_back(word);
    }
    for(int j=0; j<5; j++){
        string word;
        for(int j=0; j<u(e)+1; j++){
            word.push_back('a' + u(e));
        }
        seq.push_back(word);
    }

    for(auto&& word : seq){
        if(t.find(word)){
            cout << "found" << endl;
        } else {
            cout << "not found" << endl;
        }
    }

    for(auto&& word : seq){
        if(word.length() > 6){
            auto res = t.remove(word);
            if(res){
                cout << "remove successful" << endl;
            } else {
                cout << "remove failed" << endl;
            }
        }
    }

    for(auto&& word : seq){
        if(t.find(word)){
            cout << "found" << endl;
        } else {
            cout << "not found" << endl;
        }
    }
}

void test_math(){
    using namespace algorithm::math;
    srand(time(0));
    int base = rand() % 100;
    int pow = rand() % 20, times = rand() % 300;

    cout << base << ',' << pow << ',' << fastPow(base, pow) << endl;
    cout << base << ',' << times << ',' << fastMul(base, times) << endl;
}

void test_shared_ptr(){
    int* ptr = new int(1);
    algorithm::ptr::sharedPtr<int> a(ptr);

    std::cout << a << std::endl;

    algorithm::ptr::sharedPtr<int> b = a;

    std::cout << a << std::endl;
}

int main(){
    // test_binary_tree();
    // test_heap();
    // test_binary_search_tree();
    // test_delist();
    // test_rb_tree();
    // test_b_tree();
    // test_trie();
    // test_math();
    test_b_tree();
    // test_shared_ptr();

    return 0;
}