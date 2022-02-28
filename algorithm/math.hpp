#pragma once 


namespace algorithm::math{
    template<typename Tp_>
    Tp_ fastPow(Tp_ base, int num){
        Tp_ ans = 1;

        while(num){
            if(num & 1){
                ans *= base;
            }
            base *= base;
            num >>= 1;
        }

        return ans;
    }

    template<typename Tp_>
    Tp_ fastMul(Tp_ base, int num){
        Tp_ ans = 0;

        while(num){
            if(num & 1){
                ans += base;
            }
            base += base;
            num >>= 1;
        }

        return ans;
    }
}