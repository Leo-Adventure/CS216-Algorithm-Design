#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

int main()
{
    ll house_num, letter_num, running_time;
    cin >> house_num >> letter_num >> running_time;

    vector<ll> distance(house_num - 1, 0); // walking distance
    vector<ll> leave_time(house_num, 0);   // leave house time
    vector<ll> arrive_time(house_num, 0);  // arrive house time
    vector<ll> influ_num(house_num, 0);    // running influ letter number
    vector<ll> dest_num(house_num, 0);     // the number of letter whose destination is i
    vector<ll> letter_begin_time(letter_num, 0);
    vector<ll> latest_time(house_num, 0);
    vector<vector<ll>> dest_letters(house_num, vector<ll>(0));

    for (ll i = 0; i < house_num - 1; i++)
    {
        cin >> distance[i];
    }

    for (ll i = 0; i < letter_num; i++)
    {
        ll src, dest, time;
        cin >> src >> dest >> time;
        // 注意从零开始
        letter_begin_time[i] = time;
        latest_time[src - 1] = max(time, latest_time[src - 1]);
        dest_letters[dest - 1].push_back(i);
        dest_num[dest - 1]++;
    }

    for (ll i = 1; i < house_num; i++)
    {
        leave_time[i - 1] = max(arrive_time[i - 1], latest_time[i - 1]);
        arrive_time[i] = leave_time[i - 1] + distance[i - 1];
    }
    leave_time[house_num - 1] = max(arrive_time[house_num - 1], latest_time[house_num - 1]);
    
    ll total = 0;
    for (ll i = 0; i < house_num; i++)
    {
        ll size = dest_letters[i].size();
        for (ll j = 0; j < size; j++)
        {
            ll start = letter_begin_time[dest_letters[i][j]];
            total += arrive_time[i] - start;
        }
    }

    for (ll r = 0; r < running_time; r++)
    {

        ll sum = 0;
        ll max_idx = -1;
        ll max_val = -1;
        // i represents running between house i-1 and i.

        for (ll i = house_num - 1; i >= 0; i--)
        {
            if (arrive_time[i] > latest_time[i])
            {
                sum += dest_num[i];
            }
            else
            {
                sum = dest_num[i];
            }

            if (distance[i - 1] != 0 && sum > max_val)
            {
                max_val = sum;
                max_idx = i;
            }
        }

        if (max_idx > 0)
        {
            distance[max_idx - 1]--;
            total -= max_val;
            for (int i = max_idx; i < house_num && arrive_time[i] > latest_time[i]; i++)
            {
                arrive_time[i]--;
            }
        }
        else
        {
            break;
        }
    }

    cout << total << endl;
    return 0;
}