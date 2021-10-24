#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;
using std::max;
using std::lower_bound;

namespace algorithm::dp{
    class dp{
    public:
        static int lcs(const string& str1, const string& str2){
            int l1 = str1.length();
            int l2 = str2.length();
            vector<vector<int>> dp(l1+1, vector<int>(l2+1, 0));

            for(int i=1; i<=l1; i++){
                for(int j=1; j<=l2; j++){
                    if(str1[i] == str2[j]){
                        dp[i][j] = max(dp[i-1][j-1]+1, dp[i][j]);
                    } else {
                        dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
                    }
                }
            }

            return dp[l1][l2];
        }
    
        static int lis(const vector<int>& nums){
            vector<int> d;

            for(auto&& num : nums){
                auto iter = lower_bound(d.begin(), d.end(), num);
                if(iter != d.end()){
                    *iter = num;
                } else {
                    d.emplace_back(num);
                }
            }

            return d.size();
        }

        static int complete_bag_dp();
    };
}