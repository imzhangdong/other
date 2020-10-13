#include<bits/stdc++.h>
using namespace std;
class Solution{
    using ll = long long;
public:
    ll num;
    vector<ll> primes;
    vector<vector<ll>> dp;
 
    Solution(ll num):num(num){
        euler();
        dp.resize(primes.size(),vector<ll>(num+1,-1));
    }

    void euler(){
        bool * tmp = new bool[num+1001];
        memset(tmp,1,num+1001);
        for(ll i=2;i<=num+1000;++i){
            if(true == tmp[i]){
                for(ll j=i*i;j<=num+1000;j+=i){
                    tmp[j] = false;
                }
            }
        }
        for(ll i=2;i<=num;++i){
            if(true == tmp[i]){
                primes.push_back(i);
            }
        }
        delete[] tmp;
    }

    ll dfs(ll pos,ll sum){
        /* 三个if位置关系不能变 */
        if(sum == num) return 1;
        if(pos >= primes.size()||sum >  num) return 0;
        if(dp[pos][sum]!= -1) return dp[pos][sum];
        dp[pos][sum] = dfs(pos+1,sum)+dfs(pos+1,sum+primes[pos]);
        return dp[pos][sum];
    }
    ll dfs(){
        return dfs(0,0);
    }

    ll dpFun(){
        for(ll j= 0;j<=num;++j){
            if(j==primes[0]){
                dp[0][j] = 1;
            }else{
                dp[0][j] = 0;
            }
        }
        for(ll i=1;i<primes.size();++i){
            for(ll j=0;j<=num;++j){
                dp[i][j]=dp[i-1][j];
                if(j>primes[i]){
                    dp[i][j]+=dp[i-1][j-primes[i]];
                }
                else if(j==primes[i]){
                    dp[i][j]+=1;
                }
            }
        }
        return dp[primes.size()-1][num];
    }
};
int main(){
    
        cout <<Solution(2019).dpFun() <<endl;
}