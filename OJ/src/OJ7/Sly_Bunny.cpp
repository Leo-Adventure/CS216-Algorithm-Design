#include<iostream>
#include<vector>
using namespace std;

typedef long long ll;

const ll MOD = 1000000007;


ll quick_pow(ll a, ll b, ll mod)
{
    ll r = 1, base = a;
    while (b != 0)
    {
        if (b % 2)
            r *= base;
        r %= mod;
        base *= base;
        base %= mod;
        b /= 2;
    }
    r %= mod;
    return r;
}

ll reverse_mod(ll num, ll mod)
{
    return quick_pow(num, mod - 2, mod);
}

ll getF(ll a, ll b, ll p)
{
    ll k = a;
    k *= reverse_mod(b + p, p);
    k %= p;
    return k;
}

ll OPT(ll n, ll m, ll k);

vector<vector<ll>> process(1005, vector<ll>(1005, 1ll));

int main(){
    int T;
    cin >> T;
    process[0][0] = 0;
    for(int i = 1; i < 1005; i++){
      
        for(int j = 0; j < i; j++){
           
            if(j == 0){
                process[i][j] = 0;
            }else{
                if((process[i - 1][j - 1] + process[i - 1][j]) % 2 == 0){
                    process[i][j] = (process[i - 1][j - 1] + process[i - 1][j]) / 2;
                }else{
                    process[i][j] = getF(process[i - 1][j - 1] + process[i - 1][j], 2, MOD);
                }
            }
        }
        
        process[i][i] = i;
    }
    
    
    for(int t = 0; t < T; t++){
        ll n, m, k;
        cin >> n >> m >> k;
        // printf("n = %lld, m = %lld, k = %lld\n", n, m, k);
        cout << OPT(n, m, k) << endl;
    }
    return 0;
    
}

ll OPT(ll n, ll m, ll k){
    return (process[n][m] * k) % MOD;

/*  
// printf("n = %d, m = %d, k = %d\n", n, m, k);
    if(m == 0){
        return 0;
    }
    if(n == m){
        return (m * k) % MOD;
    }

    ll num1 = OPT(n - 1, m - 1, k);
    ll num2 = OPT(n - 1, m, k);
    if((num1 + num2) % 2 == 0){
        return ((num1 + num2) / 2) % MOD;
    }else{
        ll sum = (num1 + num2);
        return getF(sum, 2, MOD);
    }*/

}