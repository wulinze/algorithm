#pragma once

#include <iostream>
#include <memory>
#include <queue>
#include <vector>
#include <unordered_set>

using std::vector;
using std::endl;
using std::cout;
using std::unordered_set;

namespace algorithm::greedy{
    class greedy{
    public:
        template<typename T>
        using Matrix = vector<vector<T>>;

        template<typename T>
        static vector<T> Dijstrala(Matrix<T>& adj, int ori){
            int n = adj.size();
            vector<T> distance = adj[ori];
            unordered_set<int> s;
            s.insert(ori);

            while(s.size() < n){
                int k = -1;
                for(int i=0; i<n; i++){
                    if(!s.count(i) && (k==-1||distance[k] < distance[i])){
                        k = i;
                    }
                }
                s.insert(k);

                for(int i=0; i<n; i++){
                    if(!s.count(i) && distance[k] + adj[k][i] < distance[i]){
                        distance[i] = distance[k] + adj[k][i];
                    }
                }
            }

            return distance;
        }
    };
}