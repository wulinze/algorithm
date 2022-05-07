#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>
#include <queue>

using std::lower_bound;
using std::runtime_error;
using std::pair;
using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::queue;

namespace algorithm::tree{
    template<typename _Kp, typename _Vp>
    struct btree_node : public std::enable_shared_from_this<btree_node<_Kp, _Vp>>{
        vector<_Kp> keys;
        vector<_Vp> vals;
        vector<shared_ptr<btree_node<_Kp, _Vp>>> children;

        shared_ptr<btree_node<_Kp, _Vp>> parent;
        int maxSize = 0;

        bool isLeaf = true;

        shared_ptr<btree_node<_Kp, _Vp>> brother(){
            long unsigned int i=0;
            if(parent == nullptr || parent->keys.size() < 1)return nullptr;
            for(; i<parent->keys.size(); i++){
                if(this->keys[0] < parent->keys[i]){
                    break;
                }
            }

            return i==parent->keys.size()? parent->children[i-1]: parent->children[i+1];
        }

        // when node size bigger than maxSize the node should be splited
        void split(){
            if(keys.size() < static_cast<long unsigned int>(maxSize)){
                throw runtime_error("no need to split");
            }

            int t = maxSize / 2;
            // leaf node split
            auto left = make_shared<btree_node<_Kp, _Vp>>(maxSize, 
                        this->keys.begin(), this->keys.begin()+t, 
                        this->vals.begin(), this->vals.begin()+t,
                        this->parent);
            auto right = make_shared<btree_node<_Kp, _Vp>>(maxSize, 
                        this->keys.begin()+t+1, this->keys.end(), 
                        this->vals.begin()+t+1, this->vals.end(),
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
        shared_ptr<btree_node<_Kp, _Vp>> merge(shared_ptr<btree_node<_Kp, _Vp>> node, int parentIdx){
            if(!node)return nullptr;
            if(this->keys.empty()){
                if(node->keys[0] > parent->keys[parentIdx]){
                    node->keys.insert(node->keys.begin(), parent->keys.begin()+parentIdx, parent->keys.begin()+parentIdx+1);
                    node->vals.insert(node->vals.begin(), parent->vals.begin()+parentIdx, parent->vals.begin()+parentIdx+1);
                } else {
                    node->keys.insert(node->keys.end(), parent->keys.begin()+parentIdx, parent->keys.begin()+parentIdx+1);
                    node->vals.insert(node->vals.end(), parent->vals.begin()+parentIdx, parent->vals.begin()+parentIdx+1);
                }
                parent->keys.erase(parent->keys.begin()+parentIdx);
                parent->vals.erase(parent->vals.begin()+parentIdx);
                return node;
            } else if(this->keys.back() < node->keys.back()){
                if(this->keys[0] > parent->keys[parentIdx]){
                    this->keys.insert(this->keys.begin(), parent->keys.begin()+parentIdx, parent->keys.begin()+parentIdx+1);
                    this->vals.insert(this->vals.begin(), parent->vals.begin()+parentIdx, parent->vals.begin()+parentIdx+1);
                } else {
                    this->keys.insert(this->keys.end(), parent->keys.begin()+parentIdx, parent->keys.begin()+parentIdx+1);
                    this->vals.insert(this->vals.end(), parent->vals.begin()+parentIdx, parent->vals.begin()+parentIdx+1);
                }
                parent->keys.erase(parent->keys.begin()+parentIdx);
                parent->vals.erase(parent->vals.begin()+parentIdx);
                this->keys.insert(this->keys.end(), node->keys.begin(), node->keys.end());
                this->vals.insert(this->vals.end(), node->vals.begin(), node->vals.end());
                return this->shared_from_this();
            } else {
                node->merge(this->shared_from_this(), parentIdx);
                return node;
            }
        }

        // add a key and val into the node and children node
        void add(const _Kp& key, const _Vp& val, 
            shared_ptr<btree_node<_Kp, _Vp>> left = nullptr,
            shared_ptr<btree_node<_Kp, _Vp>> right = nullptr){
            if(left || right){// son split
                int pos = addKey(key, val);
                try{
                    if(children.size() < static_cast<long unsigned int>(pos+1))children.resize(pos+1);
                    children[pos] = right;
                    children.insert(this->children.begin()+pos, left);
                    left->parent = this->shared_from_this();
                    right->parent = this->shared_from_this();
                } catch (const std::exception& ex){
                    throw("parent node split failed");
                }
            } else {
                if(!isLeaf){
                    throw runtime_error("add to non-leaf node");
                } else{
                   addKey(key, val);
                   if(this->keys.size() >= static_cast<long unsigned int>(maxSize)){
                       this->split();
                   } 
                }
            }
        }

        // add a key and val
        int addKey(const _Kp& key, const _Vp& val){
            auto iter = lower_bound(this->keys.begin(), this->keys.end(), key);
            int pos = iter - this->keys.begin();

            this->keys.insert(iter, key);
            this->vals.insert(this->vals.begin()+pos, val);

            return pos;
        }

        void delKey(const _Kp& key){
            auto iter  = lower_bound(this->keys.begin(), this->keys.end(), key);
            int pos = iter - this->keys.begin();
            if(this->isLeaf){
                this->keys.erase(iter);
                this->vals.erase(this->vals.begin() + pos);
                return;
            }
            auto child = this->children[pos+1];
            auto pre = this->shared_from_this();
            while(child && !child->isLeaf){
                pre->keys[pos] = child->keys[0];
                pre->vals[pos] = child->vals[0];
                pos=0; pre = child;
                child = pre->children[pos+1];
            }

            if(child){
                pre->keys[pos] = child->keys[0];
                pre->vals[pos] = child->vals[0];
                auto brother = child->brother();
                child->delKey(child->keys[0]);
                if(child->keys.size() < static_cast<long unsigned int>(maxSize/2)) {
                    if (brother && brother->keys.size() > static_cast<long unsigned int>(maxSize/2)){
                        child->addKey(pre->keys[pos], pre->vals[pos]);
                        pre->addKey(brother->keys[0], brother->vals[0]);
                        pre->keys.erase(pre->keys.begin() + pos);
                        pre->vals.erase(pre->vals.begin() + pos);
                        brother->delKey(brother->keys[0]);
                    } else {
                        if(pre){
                            if(pos < pre->children.size()-1)pos+=1;
                            pre->children[pos] = child->merge(brother, pos);
                            pre->children.erase(pre->children.begin() + pos+1);
                            if(pre->keys.empty()){
                                child->parent = nullptr;
                            }
                        }
                    }
                }
            }

        }

        btree_node(const int& max_size,
        typename vector<_Kp>::iterator kbegin, 
        typename vector<_Kp>::iterator kend,
        typename vector<_Kp>::iterator vbegin, 
        typename vector<_Kp>::iterator vend, 
        shared_ptr<btree_node<_Kp, _Vp>> parent = nullptr):
        keys(kbegin, kend), 
        vals(vbegin, vend),
        parent(parent){
            maxSize = max_size;
            isLeaf = true;
        }

        explicit btree_node(int maxSize){
            this->maxSize = maxSize;
            parent = nullptr;
            isLeaf = true;
        }
    };
    
    template<typename _Kp, typename _Vp>
    class b_tree{
    private:
        shared_ptr<btree_node<_Kp, _Vp>> root;

        int M;
    public:
        b_tree(const int& maxSize){
            M = maxSize;
            root = make_shared<btree_node<_Kp, _Vp>>(this->M);
        }
        void insert(const _Kp& key, const _Vp& val);
        void remove(const _Kp& key);
        _Vp find(const _Kp& key);
        void level_tranverse();
    };

    template<typename _Kp, typename _Vp>
    void b_tree<_Kp, _Vp>::insert(const _Kp& key, const _Vp& val){
        // insert must be in leaf node
        if(this->root->isLeaf){
            // if root is leaf node insert should be directly use API supported by btree_node
            this->root->add(key, val);
        } else {
            auto p = this->root;
            while(!p->isLeaf){
                auto iter = lower_bound(p->keys.begin(), p->keys.end(), key);
                p = *(p->children.begin()+(iter - p->keys.begin()));
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
        if(!root){
            throw runtime_error("tree is empty");
        }
        auto p = this->root;
        auto iter = p->keys.begin();
        while(!p->isLeaf){
            iter = lower_bound(p->keys.begin(), p->keys.end(), key);
            // find in non-leaf node
            if(*iter == key){
                break;
            }
            // from up to down to find node
            p = *(p->children.begin()+(iter - p->keys.begin()));
        }
        // key may in leaf node
        iter = lower_bound(p->keys.begin(), p->keys.end(), key);
        if(iter == p->keys.end() || *iter != key){
            throw std::runtime_error("no key in btree worng remove !!!");
        }

        if(!p->isLeaf){
            p->delKey(key);
            if(root->keys.empty())root = root->children[0];
        } else {
            auto brother = p->brother();
            p->keys.erase(iter);
            p->vals.erase(p->vals.begin() + (iter - p->keys.begin()));
            if(!brother){
                if(p->keys.empty())root = nullptr;
                return;
            }
            int i;
            for(i=0; i<p->parent->keys.size(); i++){
                if(p->parent->keys[i] > brother->keys[0])break;
            }
            auto parPos = i-1;
            if(p->keys.size() < static_cast<long unsigned int>(M/2)) {
                if (brother && brother->keys.size() > static_cast<long unsigned int>(M/2)){
                    p->addKey(p->parent->keys[parPos], p->parent->vals[parPos]);
                    p->parent->addKey(brother->keys[0], brother->vals[0]);
                    p->parent->keys.erase(p->parent->keys.begin() + parPos);
                    p->parent->vals.erase(p->parent->vals.begin() + parPos);
                    brother->delKey(brother->keys[0]);
                } else {
                    if(p->parent){
                        p->parent->children[parPos] = p->merge(brother, parPos);
                        p->parent->children.erase(p->parent->children.begin() + parPos+1);
                        if(p->parent->keys.empty()){
                            p->parent = nullptr;
                            root = p;
                        }
                    }
                }
            }
        }
        
    }

    template<typename _Kp, typename _Vp>
    _Vp b_tree<_Kp, _Vp>::find(const _Kp& key){
        if(!root){
            throw runtime_error("tree is empty");
        }
        auto p = this->root;
        while(!p->isLeaf){
            auto iter = lower_bound(p->keys.begin(), p->keys.end(), key);
            // find in non-leaf node
            if(*iter == key)return *(p->vals.begin() + (iter - p->keys.begin()));
            // from up to down to find node
            p = *(p->children.begin()+(iter - p->keys.begin()));
        }
        // leaf node process
        auto iter = lower_bound(p->keys.begin(), p->keys.end(), key);
        if(*iter != key){
            std::cout << "can't find key " << key << std::endl;
            return _Vp();
        }
        
        return *(p->vals.begin() + (iter - p->keys.begin()));
    }

    template<typename _Kp, typename _Vp>
    void b_tree<_Kp, _Vp>::level_tranverse(){
        queue<shared_ptr<btree_node<_Kp, _Vp>>> q;
        if(!root){
            throw runtime_error("tree is empty");
        }

        q.push(root);
        while(!q.empty()){
            int size = q.size();
            for(int i=0; i<size; i++){
                auto top = q.front();q.pop();
                for(long unsigned int j=0; j<top->keys.size(); j++){
                    cout << "keys: " << top->keys[j] << ", vals: " << top->vals[j] << endl; 
                }
                if(top->isLeaf){
                    cout << "leaf node" << endl;
                } else {
                    cout << "non-leaf node" << endl;
                }

                for(long unsigned int j=0; j<top->children.size(); j++){
                    q.push(top->children[j]);
                }
            }
            cout << "level end" << endl;
        }
    }
}