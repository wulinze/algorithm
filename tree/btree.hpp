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
        vector<shared_ptr<btree_node<_Kp, _Vp>>> children;

        shared_ptr<btree_node<_Kp, _Vp>> parent;
        int maxSize = 0;

        bool isLeaf = true;

        shared_ptr<btree_node<_Kp, _Vp>> brother(){
            int i=0;
            for(; i<parent->keys.size(); i++){
                if(this->keys[0] > parent->keys[i]){
                    break;
                }
            }

            return i==parent->keys.size() ? parent->children[i-1] : parent->children[i];
        }

        // when node size bigger than maxSize the node should be splited
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
                this->parent->add(this->keys[t], this->vals[t], left, right);
            } else {
                this->parent = make_shared<btree_node<_Kp, _Vp>>(maxSize);
                this->parent->isLeaf = false;
                this->parent->add(this->keys[t], this->vals[t], left, right);
            }
        }

        // when key remove happen, the node may need merge
        // merge only happen in leaf node
        shared_ptr<btree_node<_Kp, _Vp>> merge(shared_ptr<btree_node<_Kp, _Vp>> node){
            if(this->keys.back() < node->keys.back()){
                this->keys.insert(this->keys.end(), node->keys.begin(), node->keys.end());
                this->vals.insert(this->vals.end(), node->vals.begin(), node->vals.end());
                return make_shared<btree_node<_Kp, _Vp>>(this);
            } else {
                node->merge(make_shared<btree_node<_Kp, _Vp>>(this));
                return node;
            }
        }

        // add a key and val into the node and children node
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

        // add a key and val
        int addKey(const _Kp& key, const& _Vp& val){
            auto iter = lower_bound(this->keys.begin(), this->keys.end(), key);
            int pos = iter - this->keys.begin();

            this->keys.insert(iter, key);
            this->vals.insert(this->vals.begin()+pos, val);

            return pos;
        }

        void delKey(const _Kp& key){
            auto iter  = lower_bound(this->keys.begin, this->keys.end(), key);
            int pos = iter - this->keys.begin();

            auto child = this->children[pos+1];
            auto pre = this;
            while(child && !child->isLeaf){
                pre->keys[pos] = child->keys[0];
                pre->vals[pos] = child->vals[0];
                pos=0; pre = child;
                child = pre->children[pos+1];
            }

            if(child){
                child->keys.erase(child->keys.begin());
                child->vals.erase(child->vals.begin());
            }
        }

        btree_node(const int& max_size, 
        const vector<_Kp>& keys, 
        const vector<_Vp>& vals,
        const vector<shared_ptr<btree_node<_Kp, _Vp>>>& children
        shared_ptr<btree_node<_Kp, _Vp>> parent = nullptr):
        keys{keys.begin(), keys.end()}, vals{vals.begin(), vals.end()},
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
    void b_tree<_Kp, _Vp>::insert(const _Kp& key, const _Vp& val){
        // insert must be in leaf node
        if(this->root.isLeaf){
            // if root is leaf node insert should be directly use API supported by btree_node
            this->root->add(key, val);
        } else {
            auto p = this->root;
            while(!p.isLeaf){
                auto iter = p->keys.lower_bound(p->keys.begin(), p->keys.end(), key);
                p = (p->children.begin()+(iter - p->keys.begin()));
            }
            p->add(key, val);
        }

        // after split leaf node from down to up
        // we need to fit the root pointer to real node
        if(this->root->parent != nullptr){
            this->root = this->root->parent;
        }
    }

    template<typename _Kp, typename _Vp>
    void b_tree<_Kp, _Vp>::remove(const _Kp& key){
        auto p = this->root;
        vector<_Kp>::iterator iter;
        while(!p->isLeaf){
            iter = p->keys.lower_bound(p->keys.begin(), p->keys.end(), key);
            // find in non-leaf node
            if(iter == p->keys.end()){
                throw std::runtime_error("Delete internal wrong");
            } else if(*iter == key){
                break;
            }
            // from up to down to find node
            p = p->children.begin()+(iter - p->keys.begin());
        }
        // key may in leaf node
        iter = p->keys.lower_bound(p->keys.begin(), p->keys.end(), key);
        if(iter == p->keys.end() || *iter != key){
            throw std::runtime_error("no key in btree worng remove !!!");
        }

        if(!p->isLeaf){
            p->delkey(key);
        } else {
            p->keys.erase(iter);
            p->vals.erase(p.vals.begin() + (iter - p.keys.begin()));
            auto brother = p->brother();
            if(p->keys.size() < M/2) {
                if (brother->keys.size() > M/2){
                    p->addKey(p->parent->keys[0], p->parent->vals[0]);
                    p->parent->addKey(brother->keys[0], brother->vals[0]);
                    p->parent->keys.erase(p->parent->keys.begin());
                    p->parent->vals.erase(p->parent->vals.begin());
                } else {
                    p->merge(brother);
                }
            }
        }
        
    }

    template<typename _Kp, typename _Vp>
    _Vp b_tree<_Kp, _Vp>::find(const _Kp& key){
        auto p = this->root;
        while(!p->isLeaf){
            auto iter = p->keys.lower_bound(p->keys.begin(), p->keys.end(), key);
            // find in non-leaf node
            if(*iter == key)return p->vals.begin() + (iter - p->keys.begin());
            // from up to down to find node
            p = p->children.begin()+(iter - p->keys.begin());
        }
        // leaf node process
        auto iter = p->keys.lower_bound(p->keys.begin(), p->keys.end(), key);
        if(*iter != key){
            std::cout << "can't find key" << std::endl;
            return _Vp();
        }
        
        return *(p->vals.begin() + (iter - p->keys.begin()));
    }
}