#pragma once

#include "binary_tree.hpp"
#include <memory>

using std::shared_ptr;

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
    void bs_tree<_Tp, _Np>::remove(const _Tp& val){}

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