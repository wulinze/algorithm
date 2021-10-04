#pragma once 

#include <iostream>
#include <memory>

using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::cout;
using std::endl;

namespace algorithm::delist{
    template<typename _Tp>
    struct deNode{
        _Tp val;

        shared_ptr<deNode<_Tp>> next, pre;

        inline deNode(const _Tp& val, 
                shared_ptr<deNode<_Tp>> pre=nullptr, 
                shared_ptr<deNode<_Tp>> next=nullptr){
            this->val = val;
            this->pre = pre;
            this->next = next;
        }
    };

    template<typename _Tp>
    class delist{
    private:
        shared_ptr<deNode<_Tp>> head, tail;

        int size=0;
    public:
        delist(){
            this->head=make_shared<deNode<_Tp>>(0);
            this->tail=make_shared<deNode<_Tp>>(0);

            this->head->next = this->tail;
            this->tail->pre = this->head;
        }

        _Tp operator[](int n){
            if(n >= size)throw std::runtime_error("size is smaller than given");

            auto p = head->next;

            for(int i=0; i<n; i++){
                p = p->next;
            }

            return p->val;
        }

        void emplace_back(const _Tp& val){
            auto newnode = make_shared<deNode<_Tp>>(val, nullptr, this->tail);
            
            if(size == 0){
                newnode->pre = head;
                head->next = newnode;
                tail->pre = newnode;
            } else {
                newnode->pre = tail->pre;
                tail->pre->next = newnode;
                tail->pre = newnode;
            }

            size ++ ;
        }

        void emplace(const _Tp& val){
            auto newnode = make_shared<deNode<_Tp>>(val, this->head);
            
            if(size == 0){
                head->next = newnode;
                tail->pre = newnode;
                newnode->next = tail;
            } else {
                head->next->pre = newnode;
                newnode->next = head->next;
                head->next = newnode;
            }

            size ++;
        }

        void pop_back(){
            if(size < 1){throw std::runtime_error("delist is empty");}

            if(size == 1){
                head->next = tail;
                tail->pre = head;
            } else {
                tail->pre = tail->pre->pre;
                tail->pre->next = tail;
            }

            size--;
        }
    
        void pop(){
            if(size < 1){throw std::runtime_error("delist is empty");}

            if(size == 1){
                head->next = tail;
                tail->pre = head;
            } else {
                head->next = head->next->next;
                head->next->pre = head;
            }

            size--;
        }

        void tranverse(){
            auto p = head->next;
            cout << "size:" << size << endl;
            while(p != tail){
                cout << p->val << ' ';
                p = p->next;
            }
            cout << endl;
        }

        void reverse_tranverse(){
            auto p = tail->pre;
            cout << "size:" << size << endl;
            while(p != head){
                cout << p->val << ' ';
                p = p->pre;
            }
            cout << endl;
        }
    
        int length(){
            return this->size;
        }
    };
}