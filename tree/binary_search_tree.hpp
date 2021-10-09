#pragma once

#include "binary_tree.hpp"
#include <memory>
#include <functional>

using std::shared_ptr;
using std::function;

namespace algorithm::tree{
    template<typename _Tp, typename _Np = tnode<_Tp>>
    class bs_tree: public tree<_Tp, _Np>{
    public:
        bs_tree() = default;
        virtual ~bs_tree() = default;

        virtual void insert(const _Tp& val);
        virtual void remove(const _Tp& val);
        virtual shared_ptr<_Np> find(const _Tp& val);
    };

    template<typename _Tp, typename _Np>
    void bs_tree<_Tp, _Np>::insert(const _Tp& val){
        if(!this->root){this->root = make_shared<_Np>(val);return;}
        auto p = this->root;

        while(p){
            if(p->val > val){
                if(!p->left){p->left=make_shared<_Np>(val);break;}
                p = p->left;
            } else{
                if(!p->right){p->right=make_shared<_Np>(val);break;}
                p = p->right;
            }
        }
    }

    template<typename _Tp, typename _Np>
    void bs_tree<_Tp, _Np>::remove(const _Tp& val){
        auto p = this->root;
        auto parent = this->root;

        while(p){
            parent = p;
            if(p->val < val) {
                p = p->right;
            } else if(p->val > val) {
                p = p->left;
            } else {
                break;
            }
        }

        auto node = p;

        if(node->left && node->right){
            auto parent = node;
            auto p = node->right;
            while(p->left){
                parent = p;
                p = p->left;
            }

            parent->val = p->val;
            if(p = parent->left){
                parent->left = p->right;
            } else {
                parent->right = p->right;
            }
        } else if(node->left){
            if(parent == p)this->root = node->left;
            else {
                if(p = parent->left)parent->left = node;
                else parent->right = node;
            }
        } else if(node->right){
            if(parent == p)this->root = node->right;
            else {
                if(p = parent->left)parent->left = node->right;
                else parent->right = node->right;
            }
        }
    }

    template<typename _Tp, typename _Np>
    shared_ptr<_Np> bs_tree<_Tp, _Np>::find(const _Tp& val){
        auto p = this->root;

        while(p){
            if(p->val < val) {
                p = p->right;
            } else if(p->val > val) {
                p = p->left;
            } else {
                return p;
            }
        }

        return nullptr;
    }
}