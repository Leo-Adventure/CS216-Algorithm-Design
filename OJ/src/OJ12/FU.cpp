#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;
using ll = long long;
const ll min_inf = numeric_limits<int>::min();

int main()
{
    ll T;
    cin >> T;
    for (ll t = 0; t < T; ++t)
    {
        ll N, M;
        cin >> N >> M;
        ll upbound = 0;
        ll lowbound = 0;
        vector<pair<ll, ll>> words((N + M), {0ll, 0ll});
        for (ll n = 0; n < N; ++n)
        {
            cin >> words[n].first >> words[n].second;
            // upbound += words[n].first;
        }
        for (ll m = 0; m < M; ++m)
        {
            ll idx = m + N;
            cin >> words[idx].first >> words[idx].second;
            words[idx].first = -words[idx].first;
            // lowbound += words[idx].first;
        }
        lowbound = 100000;
        upbound = 100000;
        // randomize (
        auto rng = std::default_random_engine{};
        std::shuffle(std::begin(words), std::end(words), rng);

        ll total_weight = abs(lowbound) + upbound + 1;

        vector<ll>dp(total_weight, min_inf);
        dp[abs(lowbound)] = 0;
        for(ll i = 0; i < N + M; i++){
            ll weight = words[i].first;
            ll value = words[i].second;
            // cout << "weight = " << weight << " and value = " << value << endl;
            if(weight < 0){ // 如果 weight < 0, 则从前往后遍历
                for(ll j = 0; j < total_weight + weight; j++){
                    if(dp[j - weight] == min_inf){
                        continue;
                    }
                    dp[j] = max(dp[j], dp[j - weight] + value);
                    
                } 
            }else{ // 如果 weight >= 0 则从后往前遍历
                for(ll j = total_weight - 1; j >= weight; j--){
                    if(dp[j - weight] == min_inf){
                        continue;
                    }
                    dp[j] = max(dp[j], dp[j - weight] + value);
                    
                }  
            }
        }

        cout << dp[abs(lowbound)] << endl; // 和等于零的位置

    }
    return 0;
}