#pragma once

#include <vector>
#include <functional>
#include <algorithm>

using std::vector;
using std::less;

namespace algorithm::heap{
    template<typename _Tp, typename _Sequence=vector<_Tp>,
            typename _Compare  = less<typename _Sequence::value_type> >
    class heap{
    private:
        _Sequence seq;
        _Compare comp;

        void adjustHeap(int index, bool down=false){
            int n = seq.size();
            if(down){
                int left = 2 * index + 1;
                int right = 2 * index + 2;
                int t = index;

                if(left < n && comp(seq[left], seq[t])){
                    t = left;
                }
                if(right < n && comp(seq[right], seq[t])){
                    t = right;
                }
                if(t == index)return;

                std::swap(seq[t], seq[index]);
                adjustHeap(t, down);
            } else {
                if(index <= 0)return;
                int parent = (index-1) / 2;

                if(index > 0 && !comp(seq[parent], seq[index])){
                    std::swap(seq[parent], seq[index]);
                    index = parent;
                }

                if(index != parent)return;
                else {
                    adjustHeap(index, down);
                }
            }
        }
    public:
        heap()=default;
        void push(const _Tp& num);
        _Tp pop();
        bool empty(){
            return seq.empty();
        }
    };
    template<typename _Tp, typename _Sequence,
            typename _Compare>
    void heap<_Tp, _Sequence, _Compare>::push(const _Tp& num){
        int index = seq.size();

        seq.emplace_back(num);

        adjustHeap(index);
    }

    template<typename _Tp, typename _Sequence,
            typename _Compare>
    _Tp heap<_Tp, _Sequence, _Compare>::pop(){
        if(seq.empty()){
           throw std::runtime_error("heap empty can't pop");
           return _Tp{};
        }
        _Tp head = seq[0];

        std::swap(seq[0], seq.back());
        seq.pop_back();

        adjustHeap(0, true);

        return head;
    }
}