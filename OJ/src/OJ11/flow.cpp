
#include <iostream>
#include <queue>
#include <vector>
#include <limits>

using namespace std;
using ll = long long;

const ll MAXN = 105;
const ll INF = numeric_limits<int>::max();

struct Edge
{
    ll from, weight;
    ll next;
};

vector<ll> levels;

vector<ll> mapper(1000, -1);
vector<ll> mapper_cached;

vector<ll> que(MAXN, 0);

ll bfs(ll s, ll t, vector<Edge *> &edge);

ll dfs(ll u, ll t, ll maxf, vector<Edge *> &edge);

int main()
{

    ll n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<Edge *> edge(2 * m + 15);
    for (int i = 0; i < 2 * m + 14; i++)
    {
        edge.at(i) = new Edge();
    }

    levels.resize(n + 15, 0);

    for (ll i = 0; i < 2 * m;)
    {
        ll u, v, f;
        cin >> u >> v >> f;
        edge.at(i)->from = v, edge.at(i)->weight = f;
        edge.at(i)->next = mapper.at(u);
        mapper.at(u) = i++;

        edge.at(i)->from = u;
        edge.at(i)->weight = 0;
        edge.at(i)->next = mapper.at(v);
        mapper.at(v) = i++;
    }
    ll ans = 0;

    while (bfs(s, t, edge))
    {
        //
        mapper_cached = mapper;
        ans += dfs(s, t, INF, edge);
    }
    cout << ans << endl;
}

ll bfs(ll s, ll t, vector<Edge *> &edge)
{
    // #ifndef NDEBUG
    //     ll the_maximum_level_val{};
    // #endif
    for (int i = 0; i < levels.size(); i++)
    {
        levels.at(i) = 0;
    }
    levels.at(s) = 1;
    int front = 0, rear = 1;

    que[front] = s;

    while (front < rear)
    {

        ll top = que[front++];
        if (top == t)
        {

            // #ifndef NDEBUG
            //             std::cout << "In this round BFS, the level value: "
            //                       << the_maximum_level_val
            //                       << std::endl;
            // #endif
            return 1;
        }
        for (ll e = mapper.at(top); e != -1; e = edge.at(e)->next)
        {
            ll v = edge.at(e)->from, f = edge.at(e)->weight;
            if (!levels.at(v) && f)
            {
                // #ifndef NDEBUG
                //                 the_maximum_level_val =
                // #endif
                levels.at(v) = levels.at(top) + 1;
                que[rear++] = v;
            }
        }
    }
    return 0;
}

ll dfs(ll u, ll t, ll maxf, vector<Edge *> &edge)
{
    if (u == t || maxf == 0)
        return maxf;
    ll ret = 0;
    for (ll &e = mapper_cached.at(u); e != -1; e = edge.at(e)->next)
    {
        ll v = edge.at(e)->from;
        ll f = edge.at(e)->weight;
        if (levels.at(u) + 1 == levels.at(v) && f)
        {
            ll Min = min(maxf - ret, f);
            f = dfs(v, t, Min, edge);
            edge.at(e)->weight -= f;
            edge.at(e ^ 1)->weight += f;
            ret += f;
            if (maxf == 0)
            {
                break;
            }
            if (ret == maxf)
                return ret;
        }
    }
    return ret;
}