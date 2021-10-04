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
    struct tnode{
        T val;

        shared_ptr<tnode<T>> left, right;

        explicit tnode(const T& val): val(val), left(nullptr), right(nullptr){}

        tnode() = default;
    };

    template<typename T, typename _Np = tnode<T>>
    class tree{
    protected:
        shared_ptr<_Np> root;
    public:
        tree(const vector<T>& seq){
            int n = seq.size();
            if(seq.empty())return;

            std::function<void(int, shared_ptr<tnode<T>>&)> dfs = 
            [&](int index, 
                shared_ptr<tnode<T>>& node){
                if(index >= n)return;
                int left = index *2 + 1;
                int right = index *2 + 2;
                
                node = make_shared<tnode<T>>(seq[index]);
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

        virtual ~tree() = default;
    };
    template<typename T, typename _Np>
    vector<T> tree<T, _Np>::preOrder(){
        stack<shared_ptr<_Np>> s;
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

    template<typename T, typename _Np>
    vector<T> tree<T, _Np>::inOrder(){
        stack<shared_ptr<_Np>> s;
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

    template<typename T, typename _Np>
    vector<T> tree<T, _Np>::postOrder(){
        stack<pair<shared_ptr<_Np>, bool>> s;
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

    template<typename T, typename _Np>
    vector<T> tree<T, _Np>::levelOrder(){
        if(!this->root)return {};
        queue<shared_ptr<_Np>> q;
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