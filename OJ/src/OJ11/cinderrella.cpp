
#include <iostream>
#include <queue>
#include <vector>
#include <limits>

using namespace std;
using ll = long long;

const ll MAXN = 20000;
const ll Z = 1000000000;
const ll INF = numeric_limits<ll>::max();

struct Edge
{
    ll to, weight;
    ll next;
};

vector<ll> levels;

vector<ll> mapper(40000, -1);
vector<ll> mapper_cached;

vector<ll> que(MAXN, 0);

ll bfs(ll s, ll t, vector<Edge *> &edge);

ll dfs(ll u, ll t, ll maxf, vector<Edge *> &edge);

int main()
{

    ll N, M;

    cin >> N >> M;
    vector<Edge *> edge(200000);

    levels.resize(MAXN, 0);

    ll cnt = 0;
    for (int i = 0; i < N; i++)
    {
        ll c1, d1, e1;
        cin >> c1 >> d1 >> e1;
        ll c2, d2, e2;
        cin >> c2 >> d2 >> e2;

        // 连接 S
        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (i * 3 + 1), edge.at(cnt)->weight = d1 + Z;
        edge.at(cnt)->next = mapper.at(0);
        mapper.at(0) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (0), edge.at(cnt)->weight = 0;
        edge.at(cnt)->next = mapper.at(i * 3 + 1);
        mapper.at(i * 3 + 1) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (i * 3 + 2), edge.at(cnt)->weight = d2 + Z;
        edge.at(cnt)->next = mapper.at(0);
        mapper.at(0) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (0), edge.at(cnt)->weight = 0;
        edge.at(cnt)->next = mapper.at(i * 3 + 2);
        mapper.at(i * 3 + 2) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (i * 3 + 2), edge.at(cnt)->weight = e1;
        edge.at(cnt)->next = mapper.at(i * 3 + 1);
        mapper.at(i * 3 + 1) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (i * 3 + 1), edge.at(cnt)->weight = 0;
        edge.at(cnt)->next = mapper.at(i * 3 + 2);
        mapper.at(i * 3 + 2) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (i * 3 + 1), edge.at(cnt)->weight = e2;
        edge.at(cnt)->next = mapper.at(i * 3 + 2);
        mapper.at(i * 3 + 2) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (i * 3 + 2), edge.at(cnt)->weight = 0;
        edge.at(cnt)->next = mapper.at(i * 3 + 1);
        mapper.at(i * 3 + 1) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (i * 3 + 3), edge.at(cnt)->weight = c1 + Z;
        edge.at(cnt)->next = mapper.at(i * 3 + 1);
        mapper.at(i * 3 + 1) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (i * 3 + 1), edge.at(cnt)->weight = 0;
        edge.at(cnt)->next = mapper.at(i * 3 + 3);
        mapper.at(i * 3 + 3) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (i * 3 + 3), edge.at(cnt)->weight = c2 + Z;
        edge.at(cnt)->next = mapper.at(i * 3 + 2);
        mapper.at(i * 3 + 2) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (i * 3 + 2), edge.at(cnt)->weight = 0;
        edge.at(cnt)->next = mapper.at(i * 3 + 3);
        mapper.at(i * 3 + 3) = cnt++;

        // 连接 T
        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (3 * N + 1), edge.at(cnt)->weight = c1 + c2 + 2 * Z;
        edge.at(cnt)->next = mapper.at(i * 3 + 3);
        mapper.at(i * 3 + 3) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (i * 3 + 3), edge.at(cnt)->weight = 0;
        edge.at(cnt)->next = mapper.at(3 * N + 1);
        mapper.at(3 * N + 1) = cnt++;
    }

    for (int i = 0; i < M; i++)
    {
        ll x, y, a, b;
        cin >> x >> y >> a >> b;
        ll x_plus, y_plus;
        if (x % 2 == 0)
        { // 偶数
            x = x + (x / 2 - 1);
            x_plus = x + 1;
        }
        else
        {
            x = x + (x / 2);
            x_plus = x + 2;
        }

        if (y % 2 == 0)
        {
            y = y + (y / 2 - 1);
            y_plus = y + 1;
        }
        else
        {
            y = y + (y / 2);
            y_plus = y + 2;
        }

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (x_plus), edge.at(cnt)->weight = a;
        edge.at(cnt)->next = mapper.at(y);
        mapper.at(y) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (y), edge.at(cnt)->weight = 0;
        edge.at(cnt)->next = mapper.at(x_plus);
        mapper.at(x_plus) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (x), edge.at(cnt)->weight = b;
        edge.at(cnt)->next = mapper.at(y_plus);
        mapper.at(y_plus) = cnt++;

        edge.at(cnt) = new Edge();
        edge.at(cnt)->to = (y_plus), edge.at(cnt)->weight = 0;
        edge.at(cnt)->next = mapper.at(x);
        mapper.at(x) = cnt++;
    }
    // cout << "Finished second" << endl;

    /*
    for (ll i = 0; i < 2 * m;)
    {
        ll u, v, f;
        cin >> u >> v >> f;
        edge.at(i)->to = v, edge.at(i)->weight = f;
        edge.at(i)->next = mapper.at(u);
        mapper.at(u) = i++;

        edge.at(i)->to = u;
        edge.at(i)->weight = 0;
        edge.at(i)->next = mapper.at(v);
        mapper.at(v) = i++;
    }
    */
    ll ans = 0;

    while (bfs(0, 3 * N + 1, edge))
    {
        mapper_cached = mapper;
        // cout << "running" << endl;
        ans += dfs(0, 3 * N + 1, INF, edge);
    }
    cout << ans - 2 * N * Z << endl;
}

ll bfs(ll s, ll t, vector<Edge *> &edge)
{
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
            return 1;
        }
        for (ll e = mapper.at(top); e != -1; e = edge.at(e)->next)
        {
            ll v = edge.at(e)->to, f = edge.at(e)->weight;
            if (!levels.at(v) && f)
            {
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
        ll v = edge.at(e)->to;
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