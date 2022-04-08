#include <iostream>
#include <math.h>

using namespace std;

typedef long long ll;

pair<ll, ll> find(ll dim, ll idx)//3, 82
{
    if (dim == 1)
    {
        ll x = (idx - 1) / 3 + 1;
        ll y = 0;
        if (idx <= 3)
        {
            y = idx;
        }
        else if (idx >= 7)
        {
            y = idx - 6;
        }
        else
        {
            y = 7 - idx;
        }
        return {x, y};
    }

    ll offset = pow(3 * pow(3, dim - 2), 2);
    ll axis_offset = pow(3, dim - 1);
    // cout << "offset = " << offset << endl;//81

    // 分区
    if (idx - offset <= 0)
    { // Area1
        return find(dim - 1, idx);
    }
    else if (idx - 2 * offset <= 0)
    { // 2
        ll related_offset = idx - 1 * offset; // 1
        pair<ll, ll> p = find(dim - 1, related_offset); //2, 1
        ll x = p.first, y = p.second;//1, 1
        return {pow(3, dim - 1) + 1 - x, y + axis_offset};// 3, 28
    }
    else if (idx - 3 * offset <= 0)
    { // 3
        ll related_offset = idx - 2 * offset;
        pair<ll, ll> p = find(dim - 1, related_offset);
        ll x = p.first, y = p.second;
        return {x, y + 2 * axis_offset};
    }
    else if (idx - 4 * offset <= 0)
    { // 4
        ll related_offset = idx - 3 * offset;
        pair<ll, ll> p = find(dim - 1, related_offset);
        ll x = p.first, y = p.second;
        return {x + axis_offset, (pow(3, dim - 1) + 1 - y) + 2 * axis_offset};
    }
    else if (idx - 5 * offset <= 0)
    { // 5

        ll related_offset = idx - 4 * offset;
        pair<ll, ll> p = find(dim - 1, related_offset);
        ll x = p.first, y = p.second;

        return {(pow(3, dim - 1) + 1 - x) + axis_offset, (pow(3, dim - 1) + 1 - y) + axis_offset};
    }
    else if (idx - 6 * offset <= 0)
    { // 6
        ll related_offset = idx - 5 * offset;
        pair<ll, ll> p = find(dim - 1, related_offset);
        ll x = p.first, y = p.second;

        return {x + axis_offset, (pow(3, dim - 1) + 1 - y)};
    }
    else if (idx - 7 * offset <= 0)
    { // 7
        ll related_offset = idx - 6 * offset;
        pair<ll, ll> p = find(dim - 1, related_offset);
        ll x = p.first, y = p.second;

        return {x + 2 * axis_offset, y};
    }
    else if (idx - 8 * offset <= 0)
    { // 8
        ll related_offset = idx - 7 * offset;
        pair<ll, ll> p = find(dim - 1, related_offset);
        ll x = p.first, y = p.second;

        return {(pow(3, dim - 1) + 1 - x) + 2 * axis_offset, y + axis_offset};
    }
    else
    { // 9
        ll related_offset = idx - 8 * offset;
        pair<ll, ll> p = find(dim - 1, related_offset);
        ll x = p.first, y = p.second;

        return {x + 2 * axis_offset, y + 2 * axis_offset};
    }
}

int main()
{
    ll dim, idx;
    cin >> dim >> idx;
    // for (int i = 162 + 82; i <= 162 + 81 + 81; i++)
    // {
    //     pair<ll, ll> p = find(3, i);
    //     cout << "i = " << i;
    //     printf(" {%d, %d}", p.first, p.second);
    //     if(i % 9 == 0){
    //         cout << endl;
    //     }
        
    // }
    pair<ll, ll> p = find(dim, idx);
    cout << p.first << " " << p.second << endl;
    return 0;
}