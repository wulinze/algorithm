#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>

using std::lower_bound;
using std::runtime_error;
using std::pair;
using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::vector;


namespace algorithm::tree {
    template <typename _Kp>
    struct bpTreeNode{
        using P = pair<shared_ptr<bpTreeNode<_Kp>>, shared_ptr<bpTreeNode<_Kp>>>;
        using Iter = vector<_Kp>::iterator;
        using IterTreeNode = vector<shared_ptr<bpTreeNode<_Kp>>>::iterator;
        
        bool isLeaf = false;
        int maxSize;
        vector<_Kp> keys;

        shared_ptr<bpTreeNode<_Kp>> parent;
        vector<shared_ptr<bpTreeNode<_Kp>>> children;

        bpTreeNode(int maxSize=0):maxSize(maxSize){}
        bpTreeNode(shared_ptr<bpTreeNode<_Kp>> parent, 
                    Iter kbegin, Iter kend,
                    IterTreeNode cbegin, IterTreeNode cend,
                    int maxSize=0, bool isLeaf=false):
        parent(parent),
        keys(kbegin, kend),
        children(cbegin, cend),
        maxSize(maxSize),
        isLeaf(isLeaf){}

        virtual ~bpTreeNode(){}

        virtual void add(_Kp&& key){
            auto iter = lower_bound(keys.begin(), keys.end(), key);
            keys.insert(iter, key);
        }

        virtual P split(){
            if(this->keys.size() < maxSize){
                throw runtime_error("no need to split");
            }

            auto pos = maxSize/2;
            if(!parent){
                this->parent = make_shared<bpTreeNode<_Kp>>(this->maxSize);
                parent->add(keys[pos]);
            } else {
                
            }
        }
    };

    template<typename _Kp, typename _Vp>
    struct bpLeafNode : bpTreeNode<_Kp>{
        vector<_Vp> vals;

        shared_ptr<bpLeafNode> next;

        virtual shared_ptr<bpTreeNode> split() override{
            
        }
    };

    template <typename _Kp, typename _Vp>
    class bpTree{

    };
}