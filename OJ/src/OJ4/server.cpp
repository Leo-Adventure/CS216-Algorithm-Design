#include <iostream>
#include <vector>
#include <limits>
#include <queue>
using namespace std;

typedef long long ll;

struct Node
{
    ll idx;
    bool isVisited;
    vector<ll> edges;
    Node() : idx(0), isVisited(0), edges(vector<ll>(0)){};
};

int main()
{
    ll point_num;
    cin >> point_num;

    ll minx = numeric_limits<int>::max();
    ll maxx = numeric_limits<int>::min();
    ll miny = numeric_limits<int>::max();
    ll maxy = numeric_limits<int>::min();

    ll maxx_idx, maxy_idx, minx_idx, miny_idx;

    vector<pair<ll, ll>> point(point_num, {0, 0});
    for (int i = 0; i < point_num; i++)
    {
        ll x, y;
        cin >> x >> y;
        point[i].first = x + y;
        point[i].second = x - y;
        if (point[i].first > maxx)
        {
            maxx = point[i].first;
            maxx_idx = i;
        }
        if (point[i].first < minx)
        {
            minx = point[i].first;
            minx_idx = i;
        }
        if (point[i].second > maxy)
        {
            maxy = point[i].second;
            maxy_idx = i;
        }
        if (point[i].second < miny)
        {
            miny = point[i].second;
            miny_idx = i;
        }
    }

    ll max_len = max(maxx - minx, maxy - miny);
    ll min_len = 1;

    vector<Node *> nodes(point_num);
    for (int i = 0; i < point_num; i++)
    {
        nodes[i] = new Node();
        nodes[i]->idx = i;
    }

    while (min_len <= max_len)
    {
        ll mid_len = (max_len + min_len) >> 1;
        // cout << "mid_len = " << mid_len << endl;

        bool flag = true;
        for (int i = 0; i < point_num; i++)
        {
            ll x = point[i].first;
            ll y = point[i].second;
            nodes[i]->isVisited = 0;
            ll x_len = max(x - minx, maxx - x);
            ll y_len = max(y - miny, maxy - y);
            ll len = max(x_len, y_len);
            // cout << "len = " << len << endl;
            if (len < mid_len)
            { // mid_len 太大了
                max_len = mid_len - 1;
                // cout << "In line " << __LINE__ << endl;
                flag = false;
                break;
            }
            else
            {
                if (mid_len <= x - minx)
                {
                    // cout << "In line " << __LINE__ << endl;
                    nodes[minx_idx]->edges.push_back(i);
                    nodes[i]->edges.push_back(minx_idx);
                }
                if (mid_len <= maxx - x)
                {
                    // cout << "In line " << __LINE__ << endl;
                    nodes[maxx_idx]->edges.push_back(i);
                    nodes[i]->edges.push_back(maxx_idx);
                }
                if (mid_len <= y - miny)
                {
                    // cout << "In line " << __LINE__ << endl;
                    nodes[miny_idx]->edges.push_back(i);
                    nodes[i]->edges.push_back(miny_idx);
                }
                if (mid_len <= maxy - y)
                {
                    // cout << "In line " << __LINE__ << endl;
                    nodes[maxy_idx]->edges.push_back(i);
                    nodes[i]->edges.push_back(maxy_idx);
                }
            }
        }



        queue<ll> que;
        que.push(0);
        nodes[0]->isVisited = 1;
        while (!que.empty())
        {
            ll idx = que.front();
            que.pop();
            ll size = nodes[idx]->edges.size();
            for (int s = 0; s < size; s++)
            {
                ll child_idx = nodes[idx]->edges[s];
                if (!nodes[child_idx]->isVisited)
                {
                    nodes[child_idx]->isVisited = 1;
                    que.push(child_idx);
                }
            }
        }

        for (int i = 0; i < point_num; i++)
        {
            if (!nodes[i]->isVisited)
            {
                max_len = mid_len - 1;
                // cout << "In line " << __LINE__ << endl;
                flag = false;
                break;
            }
        }

        if (flag)
        {
            min_len = mid_len + 1;
            // cout << "In line " << __LINE__ << endl;
        }

        for (int i = 0; i < point_num; i++)
        {
            nodes[i]->edges.clear();
        }
      

    }
    cout << min_len - 1 << endl;
    return 0;
}