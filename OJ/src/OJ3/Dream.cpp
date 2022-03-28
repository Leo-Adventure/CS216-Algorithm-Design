#include <iostream>
#include <vector>
#include <queue>
#include <set>

typedef long long ll;

using namespace std;

const ll inf = 100000000007;


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

ll getF(ll num, ll p)
{
    ll k = 1 + num;
    k *= reverse_mod(1 - num + p, p);
    k %= p;
    return k;
}

struct Node
{
    int idx;
    vector<vector<ll>> weight;
    vector<Node *> edges;
    Node() : idx(0), weight(0, vector<ll>(4, 0)), edges(vector<Node *>(0)){};
};

int main()
{
    // cout << " " << getF(0, 5) << " " << getF(1, 5) << " " << getF(2, 5) << " " << getF(3, 5) << " " <<getF(4, 5) << " "<< endl;
    int node_num, edge_num, mod;
    
    // cout << getF(3, 13) << " " << getF(5, 13) << " " << getF(7, 13) << " " << getF(8, 13) << " " << getF(10, 13) << " " << getF(12, 13) << endl;

    cin >> node_num >> edge_num >> mod;

    vector<Node *> nodes(node_num);

    for (int i = 0; i < node_num; i++)
    {
        nodes[i] = new Node();
        nodes[i]->idx = i;
    }

    for (int i = 0; i < edge_num; i++)
    {
        int idx1, idx2, w;
        cin >> idx1 >> idx2 >> w;
        nodes[idx1 - 1]->edges.push_back(nodes[idx2 - 1]); // 从零开始

        vector<ll> wei(4, 0);

        for (int j = 1; j < 4; j++)
        {
            wei[j] = w;
            w = getF(w, mod);
        }
        wei[0] = w;
        
        nodes[idx1 - 1]->weight.push_back(wei);
    }

    priority_queue<pair<ll, pair<ll, ll>>, vector<pair<ll, pair<ll, ll>>>, greater<pair<ll, pair<ll, ll>>>> q;
    q.push({0, {0, 0}}); // {dist, {idx, level}}

    vector<ll> dist(node_num, inf);
    vector<vector<ll>> dp(4, vector<ll>(node_num, inf));

    dist[0] = 0;
    dp[0][0] = 0;

    set<pair<ll, ll>> bool_set;

    while (!q.empty())
    {
        ll distance = q.top().first;
        ll idx = q.top().second.first;
        ll level = q.top().second.second;

        q.pop();

        if (bool_set.count({idx, level}))
        {
            continue;
        }

        bool_set.insert({idx, level});

        int child_num = nodes[idx]->edges.size();

        for (int i = 0; i < child_num; i++)
        {

            ll child_weight = nodes[idx]->weight[i][(level + 1) % 4];
            ll child_idx = nodes[idx]->edges[i]->idx;

            if (distance + child_weight < dp[(level + 1) % 4][child_idx])
            {
                dp[(level + 1) % 4][child_idx] = distance + child_weight;
                
                q.push({dp[(level + 1) % 4][child_idx], {child_idx, (level + 1) % 4}});

                if (dp[(level + 1) % 4][child_idx] < dist[child_idx])
                {
                    dist[child_idx] = dp[(level + 1) % 4][child_idx];
                }
            }
        }
    }

    ll res = dist[node_num - 1];
    cout << res << endl;
    return 0;
}