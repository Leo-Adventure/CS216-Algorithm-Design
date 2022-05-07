#include <iostream>
#include <vector>
#include <limits>

using namespace std;
using ll = long long;

const int inf = numeric_limits<int>::max();

struct Edge
{
    int from;
    int to;
    ll weight;
    Edge() : from(0), to(0), weight(0ll){};
};
int main()
{
    int spot_num, edge_num, op_num, start_spot;
    cin >> spot_num >> edge_num >> op_num >> start_spot;

    vector<ll> value(spot_num);
    vector<Edge *> edges(edge_num);

    for (int i = 0; i < spot_num; i++)
    {
        cin >> value[i];
    }

    for (int i = 0; i < edge_num; i++)
    {
        int first, second;
        ll w;
        cin >> first >> second >> w;
        edges[i] = new Edge();
        edges[i]->from = first - 1;
        edges[i]->to = second - 1;
        edges[i]->weight = w;
    }

    vector<vector<ll>> f(vector<vector<ll>>(op_num * 2 + 1, vector<ll>(spot_num, inf)));

    for (int i = 0; i <= 2 * op_num; i++)
    {
        f[i][start_spot - 1] = 0;
    }
    // bellman-ford algorithm

    for (int t = 0; t <= 2 * op_num; t++)
    {

        if (t % 2 && t > 0)
        {
            for (int i = 0; i < spot_num; i++)
            {
                f[t][i] = f[t - 1][i] + value[i];
            }
        }
        else if (t > 0)
        {
            for (int i = 0; i < spot_num; i++)
            {
                f[t][i] = f[t - 1][i] - value[i];
            }
        }

        for (int i = 0; i < spot_num; i++)
        {
            bool flag = false;
            for (int j = 0; j < edge_num; j++)
            {
                int from = edges[j]->from;
                int to = edges[j]->to;
                ll weight = edges[j]->weight;
                if (f[t][from] + weight < f[t][to])
                {
                    flag = true;
                    f[t][to] = f[t][from] + weight;

                    if (i == spot_num - 1)
                    {
                        cout << "INVINCIBLE" << endl;
                        return 0;
                    }
                }
            }
            if (!flag)
            {
                break;
            }
        }
       
    }

    ll res = f[0][0];
    for(int i = 0; i <= 2 * op_num; i++){
        for(int j = 0; j < spot_num; j++){
            res = min(res, f[i][j]);
        }
    }
    
    std::cout << -res << endl;
    return 0;
}