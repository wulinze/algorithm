#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>

using std::lower_bound;
using std::runtime_error;
using std::pair;
using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::vector;

namespace algorithm::tree{
    template<typename _Kp, typename _Vp>
    struct btree_node {
        vector<_Kp> keys;
        vector<_Vp> vals;
        vector<shared_ptr<btree_node<_Kp, _VP>>> children;

        shared_ptr<btree_node<_Kp, _VP>> parent;
        int maxSize = 0;

        bool isLeaf = true;

        void split(){
            if(keys.size() < maxSize){
                throw runtime_error("no need to split");
            }

            int t = maxSize / 2;
            
            // leaf node split
            auto left = make_shared<btree_node<_Kp, _Vp>>(maxSize, 
                        {this->keys.begin(), this->keys.begin()+t}, 
                        {this->vals.begin(), this->vals.begin()+t}, 
                        {this->children.begin(), this->children.begin()+t+1},
                        this->parent);
            auto right = make_shared<btree_node<_Kp, _Vp>>(maxSize, 
                        {this->keys.begin()+t+1, this->keys.end()}, 
                        {this->vals.begin()+t+1, this->vals.end()}, 
                        {this->children.begin()+t+2, this->children.end()},
                        this->parent);

            if(this->parent){
                this->parent.add(this->keys[t], this->vals[t], left, right);
            } else {
                this->parent = make_shared<btree_node<_Kp, _Vp>>(maxSize);
            }
        }

        void add(_Kp& key, _Vp& val, 
            shared_ptr<btree_node<_Kp, _Vp>> left = nullptr,
            shared_ptr<btree_node<_Kp, _Vp>> right = nullptr){
            if(left || right){// son split
                int pos = addKey(key, val);
                try{
                    children[pos] = right;
                    children.insert(this->children.begin()+pos, left);
                } catch (const std::exception& ex){
                    throw("parent node split failed");
                }
            } else {
                if(!isLeaf){
                    throw runtime_error("add to non-leaf node");
                } else{
                   addKey(key, val);
                   if(this->keys.size() >= maxSize){
                       this->split();
                   } 
                }
            }
        }

        int addKey(const _Kp& key, const& _Vp& val){
            auto iter = lower_bound(this->keys.begin(), this->keys.end());
            int pos = iter - this->keys.begin();

            this->keys.insert(iter, key);
            this->vals.insert(this->vals.begin()+pos, val);

            return pos;
        }

        btree_node(const int& max_size, 
        const vector<_Kp>& keys, 
        const vector<_Vp>& vals,
        const vector<shared_ptr<btree_node<_Kp, _Vp>>>& children
        shared_ptr<btree_node<_Kp, _Vp>> parent = nullptr):
        keys{keys.begin(), keys.end()},vals{vals.begin(), vals.end()},
        children{children.begin(), children.end()}, parent(parent){
            maxSize = max_size;
            isLeaf = true;
        }

        explicit btree_node(const int& maxSize){
            this->maxSize = maxSize;
            parent = nullptr;
            isLeaf = true;
        }
    };
    
    template<typename _Kp, typename _Vp>
    class b_tree{
    private:
        shared_ptr<btree_node<_Kp, _Vp>> root;

        const int M;
    public:
        b_tree(const int& maxSize){
            M = maxSize;
            root = make_shared<btree_node<_Kp, _Vp>>(this->M);
        }
        void insert(const _Kp& key, const _Vp& val);
        void remove(const _Kp& key);
        _Vp find(const _Kp& key);
    };

    template<typename _Kp, typename _Vp>
    void b_tree::insert(const _Tp& key, const _Vp& val){
        if(this->root.isLeaf){
            this->root->add(key, val);
        } else {
            auto p = this->root;
            while(!p.isLeaf){
                auto iter = p->keys.lower_bound(p->keys.begin(), p->keys.end(), key);
                p = (p->children.begin()+(iter - p->keys.begin()));
            }
            p->add(key, val);
        }
    }

    template<typename _Kp, typename _Vp>
    void b_tree::remove(const _Kp& key){
        
    }

    template<typename _Kp, typename _Vp>
    _Vp b_tree::find(const _Kp& key){
        auto p = this->root;
        while(!p.isLeaf){
            auto iter = p->keys.lower_bound(p->keys.begin(), p->keys.end(), key);
            p = p->children.begin()+(iter - p->keys.begin());
        }
        auto iter = p->keys.lower_bound(p->keys.begin(), p->keys.end(), key);
        
        return p->vals.begin() + (iter - p->keys.begin());
    }
}