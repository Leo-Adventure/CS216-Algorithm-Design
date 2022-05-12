#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;
using ll = long long;

struct qianqian
{
    template <typename T>
    size_t operator()(T const &a) const
    {
        size_t ans{};
        for (auto k : a)
        {
            ans += k;
            ans *= 50;
        }
        return ans;
    }
};
unordered_map<vector<ll>, bool, qianqian> map;

int dp(vector<ll> number, vector<vector<ll>> &cards)
{
    bool can_continue = false;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = i; j < 4; ++j)
        {
            if (number[i] != 0 && number[j] != 0 && cards[i][j] != -1)
            {
                if (i == j && number[i] < 2)
                {
                    continue;
                }
                can_continue = 1;
            }
        }
    }
    if (!can_continue)
    {
        return 0;
    }

    bool can_win = false;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = i; j < 4; ++j)
        {
            if (number[i] != 0 && number[j] != 0 && cards[i][j] != -1)
            {
                if (i == j && number[i] < 2)
                {
                    continue;
                }
                number[i]--;
                number[j]--;
                number[cards[i][j]]++;
                int is_zero = 0;
                if (map.count(number))
                {
                    is_zero = !(map[number]);
                }
                else
                {
                    is_zero = !(dp(number, cards));
                    map[number] = !is_zero;
                }

                if (is_zero)
                {
                    can_win = 1;
                    goto outside;
                }
                number[i]++;
                number[j]++;
                number[cards[i][j]]--;
            }
        }
    }

outside:
    if (can_win)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int main()
{
    vector<vector<ll>> cards(4, vector<ll>(4, 0));

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            cin >> cards[i][j];
        }
    }

    int round = 0;
    cin >> round;
    for (int r = 0; r < round; ++r)
    {

        vector<ll> number(4, 0);
        cin >> number[0] >> number[1] >> number[2] >> number[3];
        if (dp(number, cards))
        {
            cout << "Satori\n";
        }
        else
        {
            cout << "irotaS\n";
        }
    }
}