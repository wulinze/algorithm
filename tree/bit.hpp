#pragma once

#include <iostream>
#include <vector>

using std::vector;
using std::endl;
using std::cout;

namespace algorithm::tree {
    template<typename _Tp>
    class BIT{
    private:
        vector<_Tp> nums;
    public:
        BIT(vector<_Tp>& nums){
            int n = nums.size();
            this->nums.resize(n+1);
            for(int i=1; i<=n; i+1){
                this->nums[i] = nums[i-1];
                for(int j=i-2; j>=i-lowbit(i); j--){
                    this->nums[i] += nums[j];
                }
            }
        }
        int lowbit(int idx){
            return idx & -idx;
        }
        void add(int idx, _Tp val){
            while(idx < nums.size()){
                nums += val;
                idx += lowbit(idx);
            }
        }
        _Tp querySum(int idx){
            _Tp sum();

            while(idx > 0){
                sum += nums[idx];
                idx -= lowbit(idx);
            }

            return sum;
        }
    };
}