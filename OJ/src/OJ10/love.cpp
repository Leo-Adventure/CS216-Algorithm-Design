#include <iostream>
#include <vector>
#include <math.h>
#include <map>
#include <unordered_map>
#include <chrono>

using namespace std;
using namespace std::chrono;
using ll = long long;
using dtype = decltype(system_clock::now() - system_clock::now());

dtype com_cost_time;
dtype pow_cost_time;

struct hash_my_pair
{
    template <typename T>
    size_t operator()(T const &my_pair) const
    {
        return (my_pair.first << 32) ^ my_pair.second;
    }
};

unordered_map<pair<ll, ll>, ll, hash_my_pair> C_map;
unordered_map<ll, ll> fac_map;
unordered_map<pair<ll, ll>, ll, hash_my_pair> pow_map;

ll fac(ll n, ll mod)
{
    if (fac_map.count(n))
    {
        return fac_map[n];
    }
    ll res = 1;
    for (int i = 1; i <= n; i++)
    {
        res *= i;
        res %= mod;
    }
    res %= mod;
    fac_map[n] = res;
    return res;
}

ll pascal(ll row, ll column, ll mod, vector<vector<ll>> &nums)
{

    if (row < 0 || column < 0 || column > row)
    {
        nums[row][column] = 0;
        return 0;
    }

    if (row == 0 && column == 0)
    {
        nums[row][column] = 1;
        return 1;
    }

    ll res = (nums[row - 1][column] + nums[row - 1][column - 1]) % mod;
    // ll res = (pascal(row - 1, column, mod, nums) + pascal(row - 1, column - 1, mod, nums)) % mod;
    nums[row][column] = res;
    return res;
}

ll C(ll n, ll m, ll mod)
{
    if (C_map.count({n, m}))
    {
        return C_map[{n, m}];
    }
    ll fac_n = fac(n, mod);
    ll fac_n_m = fac(n - m, mod);
    ll fac_m = fac(m, mod);
    ll tmp = (fac_n_m * fac_m) % mod;

    ll res = fac_n / tmp;
    C_map[{n, m}] = res;
    return res;
}

void print(vector<vector<ll>> nums)
{
    int row = nums.size();
    int col = nums[0].size();
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            cout << nums[i][j] << " ";
        }
        cout << endl;
    }
}

ll power(ll base, ll exp, ll mod)
{
    if (pow_map.count({base, exp}))
    {
        return pow_map[{base, exp}];
    }
    ll res = 1;
    for (ll i = 0; i < exp; i++)
    {
        res *= base;
        res %= mod;
    }
    pow_map[{base, exp}] = res;
    return res;
}

int main()
{

    ll ballon_num, modulo;
    cin >> ballon_num >> modulo;
    // auto time1 = system_clock::now();
    vector<vector<ll>> opt(ballon_num + 1, vector<ll>(ballon_num + 1, 0));
    vector<vector<ll>> nums(ballon_num + 1, vector<ll>(ballon_num + 1, 1));
    vector<ll> powers(ballon_num + 1, 1);
    // auto time1 = system_clock::now();
    for (int i = 1; i < ballon_num + 1; i++)
    {
        for (int j = 1; j < i; j++)
        {
            nums[i][j] = (nums[i - 1][j] + nums[i - 1][j - 1]) % modulo;
        }
    }
    for (int i = 1; i < ballon_num; i++)
    {
        powers[i] = (2 * powers[i - 1]) % modulo;
    }
    // auto time2 = system_clock::now();
    // com_cost_time = time2 - time1;
    // cout << "calculate the nums: ";
    // cout << duration_cast<chrono::nanoseconds>(com_cost_time).count() / 1e6 << endl;

    for (int i = 1; i <= ballon_num; i++)
    {
        opt[i][i - 1] = powers[i - 1];
        opt[i][i - 1] %= modulo;
    }

#ifdef DEBUG
    auto now = std::chrono::system_clock::now();
#endif
    int cnt = 0;
    for (int i = 1; i <= ballon_num; i++) // 1 2 3 4
    {
        for (int j = 0; j < i; j++) // 0
        {
            for (register int k = 1; i + k + 1 <= ballon_num; k++) // i + k = 2 3 4
            {
                cnt++;
                // auto time1 = system_clock::now();
                long long combination_number_v = nums[j + k + 1][k]; // pascal(j + k + 1, k , modulo);
                // auto time2 = system_clock::now();
                // com_cost_time += time2 - time1;
                // time1 = system_clock::now();
                long long pow_number_v = powers[k - 1];
                // time2 = system_clock::now();
                // pow_cost_time += time2 - time1;

                ll tmp = (((opt[i][j] * combination_number_v)) % modulo) * pow_number_v % modulo;
                opt[i + k + 1][j + k] += tmp;
                // opt[i + k + 1][j + k] %= modulo;
                if (opt[i + k + 1][j + k] >= modulo)
                {
                    opt[i + k + 1][j + k] -= modulo;
                }
            }
        }
        // #ifdef DEBUG
        //         auto cost_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
        //                              std::chrono::system_clock::now() - now)
        //                              .count() /
        //                          1e6;
        //         std::clog << "Round " << i << ": cost time: " << cost_time << " ms. \n";
        // #endif
    }
    ll res = 0;
    for (int i = 0; i < ballon_num; i++)
    {
        res += opt[ballon_num][i];
        res %= modulo;
    }
    // cout << "The com_cost_time is ";
    // cout << duration_cast<chrono::nanoseconds>(com_cost_time).count() / 1e6 << endl;
    // cout << "The pow_cost_time is ";
    // cout << duration_cast<chrono::nanoseconds>(pow_cost_time).count() / 1e6 << endl;
    // cout << "cnt = " << cnt << endl;
    cout << res << endl;
    // auto time2 = system_clock::now();
    // auto&& total_time = time2 - time1;
    // cout << duration_cast<chrono::nanoseconds>(total_time).count() / 1e6 << endl;
    return 0;
}