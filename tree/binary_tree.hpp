#pragma once

#include <vector>
#include <memory>
#include <stack>
#include <queue>
#include <functional>
#include <utility>
#include <iostream>
#include <random>

using std::vector;
using std::shared_ptr;
using std::stack;
using std::queue;
using std::unique_ptr;
using std::make_shared;
using std::pair;

namespace algorithm::tree{
    template<typename T>
    struct Node{
        T val;

        shared_ptr<Node<T>> left, right;

        Node(const T& val): val(val), left(nullptr), right(nullptr){}
        Node() = default;
    };

    template<typename T>
    class tree{
    protected:
        shared_ptr<Node<T>> root;
    public:
        tree(const vector<T>& seq){
            int n = seq.size();
            if(seq.empty())return;

            std::function<void(int, shared_ptr<Node<T>>&)> dfs = 
            [&](int index, 
                shared_ptr<Node<T>>& node){
                if(index >= n)return;
                int left = index *2 + 1;
                int right = index *2 + 2;
                
                node = make_shared<Node<T>>(seq[index]);
                dfs(left, node->left);dfs(right, node->right);
            };
            
            dfs(0, this->root);
        }
        tree() = default;

        // tranverse
        vector<T> preOrder();
        vector<T> inOrder();
        vector<T> postOrder();
        vector<T> levelOrder();
    };
    template<typename T>
    vector<T> tree<T>::preOrder(){
        stack<shared_ptr<Node<T>>> s;
        vector<T> seq;
        auto p = this->root;

        while(p || !s.empty()){
            if(p){
                seq.emplace_back(p->val);
                s.push(p);
                p = p->left;
            } else {
                p = s.top()->right;s.pop();
                // p = p->right;
            }
        }

        return seq;
    }

    template<typename T>
    vector<T> tree<T>::inOrder(){
        stack<shared_ptr<Node<T>>> s;
        vector<T> seq;
        auto p = this->root;

        while(p || !s.empty()){
            if(p){
                s.push(p);
                p = p->left;
            } else {
                p = s.top();s.pop();
                seq.emplace_back(p->val);
                p=p->right;
            }
        }

        return seq;
    }

    template<typename T>
    vector<T> tree<T>::postOrder(){
        stack<pair<shared_ptr<Node<T>>, bool>> s;
        vector<T> seq;
        auto p = this->root;

        while(p || !s.empty()){
            if(p){
                s.push(make_pair(p, false));
                p = p->left;
            } else {
                p = s.top().first;
                if(s.top().second){
                    s.pop();
                    seq.emplace_back(p->val);
                    p = nullptr;
                } else {
                    s.top().second = true;
                    p = p->right;
                }
            }
        }

        return seq;
    }

    template<typename T>
    vector<T> tree<T>::levelOrder(){
        if(!this->root)return {};
        queue<shared_ptr<Node<T>>> q;
        q.push(this->root);
        vector<T> seq;

        while(!q.empty()){
            int size = q.size();
            for(int _=0; _<size; _++){
                auto t = q.front();q.pop();
                seq.emplace_back(t->val);
                if(t->left)q.push(t->left);
                if(t->right)q.push(t->right);
            }
        }

        return seq;
    }
}