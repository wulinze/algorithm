#pragma once 

#include <iostream>
#include <memory>
#include <vector>
#include <functional>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::function;
using std::runtime_error;

namespace algorithm::tree{
    enum class Color{
        RED, BLACK
    };

    template<typename _Tp>
    struct rb_node{
        _Tp val;

        Color color;

        shared_ptr<rb_node<_Tp>> parent;
        shared_ptr<rb_node<_Tp>> left, right;

        explicit rb_node(const _Tp& val, Color c = Color::RED,
            shared_ptr<rb_node<_Tp>> left=nullptr, 
            shared_ptr<rb_node<_Tp>> right=nullptr)
            : val(val), color(c), parent(nullptr), left(left), right(right){
        }
    };

    template<typename _Tp, typename _Np = rb_node<_Tp>>
    class rb_tree : public bs_tree<_Tp, _Np>{
    private:
        shared_ptr<_Np> nil = make_shared<_Np>(-1, Color::BLACK);

        void insert_fix(shared_ptr<_Np>& cur){
            function<shared_ptr<_Np> (shared_ptr<_Np>& node)> uncleNode = [](shared_ptr<_Np>& cur){
                auto parent = cur->parent;

                if(parent == cur->parent->parent->left){
                    return cur->parent->parent->right;
                } else if(parent == cur->parent->parent->right){
                    return cur->parent->parent->left;
                } else {
                    throw std::runtime_error("Wrong RED_BLACK tree structure");
                }
            };

            if(cur == this->root)return;
            while(cur->parent->color == Color::RED){
                auto uncle = uncleNode(cur);
                auto parent = cur->parent;
                auto grandParent = cur->parent->parent;

                if(uncle->color == Color::RED){
                    uncle->color = Color::BLACK;
                    parent->color = Color::BLACK;
                    if(grandParent != this->root){
                        grandParent->color = Color::RED;
                        cur = grandParent;
                    }
                    else return;

                } else if(uncle->color == Color::BLACK){
                    if(cur == parent->right){
                        if(parent == grandParent->right){
                            parent->color = Color::BLACK;

                            grandParent->color = Color::RED;
                            this->left_rotate(grandParent);
                            return;
                        } else if(parent == grandParent->left){
                            cur = parent;
                            this->left_rotate(cur);
                        }
                    } else if(cur == parent->left){
                        if(parent == grandParent->left){
                            parent->color = Color::BLACK;

                            grandParent->color = Color::RED;
                            this->right_rotate(grandParent);
                            return;
                        } else if(parent == grandParent->right){
                            cur = parent;
                            this->right_rotate(cur);
                        }
                    } else {
                        throw std::runtime_error("inner error");
                    }
                } else {
                    throw std::runtime_error("Wrong RED_BLACK tree structure");
                }
            }
        }

        void left_rotate(shared_ptr<_Np>& cur){
            auto parent = cur->parent;
            auto t = cur->right;

            if(!parent){
                cur->right = t->left;
                cur->right->parent = cur;
                t->left = cur;
                t->left->parent = t;
                this->root = t;
                this->root->parent = nullptr;
                return;
            }
            if(cur == parent->left){
                parent->left = t;
            } else if(cur == parent->right){
                parent->right = t;
            } else {
                throw std::runtime_error("cur is not a child of parent");
            }
            t->parent = parent;
            cur->right = t->left;
            cur->right->parent = cur;
            t->left = cur;
            t->left->parent = t;
        }
        void right_rotate(shared_ptr<_Np>& cur){
            auto parent = cur->parent;
            auto t = cur->left;

            if(!parent){
                cur->left = t->right;
                cur->left->parent = cur;
                t->right = cur;
                t->right->parent = t;
                this->root = t;
                this->root->parent = nullptr;
                return;
            }
            if(cur == parent->left){
                parent->left = t;
            } else if(cur == parent->right){
                parent->right = t;
            } else {
                throw std::runtime_error("cur is not a child of parent");
            }
            t->parent = parent;
            cur->left = t->right;
            cur->left->parent = cur;
            t->right = cur;
            t->right->parent = t;
        }
    public:
        void insert(const _Tp& val) override{
            shared_ptr<_Np> newnode = make_shared<_Np>(val, Color::RED, nil, nil);
            if(!this->root){this->root = make_shared<_Np>(val, Color::BLACK, nil, nil);return;}
            auto p = this->root;

            while(p != nil){
                if(p->val > val){
                    if(p->left == nil){
                        p->left=make_shared<_Np>(val, Color::RED, nil, nil);
                        p->left->parent = p;
                        p=p->left;
                        break;
                    }
                    p = p->left;
                } else{
                    if(p->right == nil){
                        p->right=make_shared<_Np>(val, Color::RED, nil, nil);
                        p->right->parent = p;
                        p=p->right;
                        break;
                    }
                    p = p->right;
                }
            }
            this->insert_fix(p);
        }
        void remove(const _Tp& val) override{

        }
    };
}