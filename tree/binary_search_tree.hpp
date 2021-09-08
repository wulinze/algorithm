#pragma once

#include "binary_tree.hpp"
#include <memory>

using std::shared_ptr;

namespace algorithm::tree{
    template<typename _Tp>
    class bs_tree: public tree<_Tp>{
    public:
        bs_tree() = default;
        void insert(const _Tp& val);
        shared_ptr<Node<_Tp>> find(const _Tp& val);
    };

    template<typename _Tp>
    void bs_tree<_Tp>::insert(const _Tp& val){
        if(!this->root){this->root = make_shared<Node<_Tp>>(val);return;}
        auto p = this->root;

        while(p){
            if(p->val > val){
                if(!p->left){p->left=make_shared<Node<_Tp>>(val);break;}
                p = p->left;
            } else{
                if(!p->right){p->right=make_shared<Node<_Tp>>(val);break;}
                p = p->right;
            }
        }
    }

    template<typename _Tp>
    shared_ptr<Node<_Tp>> bs_tree<_Tp>::find(const _Tp& val){
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