#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using ll = long long;

ll n, T;

const ll MAXN = 500005;

vector<ll> pre_sum(MAXN);
void update_right(ll l, ll r);
void divide_and_conquer(ll l, ll r);

struct point
{
    ll x, y, id;
    point(){};
    point(ll X, ll Y)
    {
        x = X;
        y = Y;
        id = 0;
    };
    int operator<(const point rhs) const
    {
        return x < rhs.x;
    }
} p[MAXN];

bool low_cmp(point &p1, point &p2)
{
    if (p1.x == p2.x)
    {
        return p1.id < p2.id;
    }

    return p1.x < p2.x;
}
bool high_cmp(point &p1, point &p2)
{
    if (p1.x == p2.x)
    {
        return p1.id < p2.id;
    }
    return p1.x > p2.x;
}

ll lowbit(ll x)
{
    return x & (-x);
}

struct u_tree
{
    ll val_arr[MAXN];
    ll size = 0;
    // 清空污染值
    void clear(ll up)
    {
        size = up;
        for (int i = 0; i <= size; i++)
        {
            val_arr[i] = numeric_limits<int>::max();
        }
    }
    // 更新树状数组
    void update(ll i, ll val)
    {
        while (i <= size)
        {
            val_arr[i] = min(val_arr[i], val);
            i += lowbit(i);
        }
    }
    ll query(ll idx)
    {
        ll ret = numeric_limits<int>::max();
        while (idx)
        {
            ret = min(ret, val_arr[idx]);
            idx -= lowbit(idx);
        }
        return ret;
    }
} u_tree;

struct d_tree
{
    ll val_arr[MAXN];
    int size = 0;
    // 清空污染值
    void clear(ll up)
    {
        size = up;
        for (int i = 0; i <= size; i++)
        {
            val_arr[i] = numeric_limits<int>::max();
        }
    }
    // 更新树状数组
    void update(ll i, ll val)
    {
        while (i)
        {
            val_arr[i] = min(val_arr[i], val);
            i -= lowbit(i);
        }
    }
    ll query(ll idx)
    {
        ll ret = numeric_limits<int>::max();
        while (idx <= size)
        {
            ret = min(val_arr[idx], ret);
            idx += lowbit(idx);
        }

        return ret;
    }
} d_tree;

vector<point> points(MAXN);
vector<ll> opt(MAXN);
vector<ll> map_arr(MAXN);

struct node
{
    ll y, id;
    node() {}
    node(ll Y, ll ID)
    {
        y = Y;
        id = ID;
    };
    int operator<(node rhs)
    {
        return y < rhs.y;
    }
};

vector<node> nodes(MAXN);

int main()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> p[i].x;
        cin >> p[i].y;
        if (i > 1)
        {
            pre_sum[i] = pre_sum[i - 1] + abs(p[i - 1].x - p[i].x) + abs(p[i - 1].y - p[i].y);
        }
    }

    opt[1] = 0;
    // 初始化
    for (int i = 2; i <= n; i++)
    {
        opt[i] = pre_sum[i - 1];
    }

    divide_and_conquer(2, n);

    ll ans = numeric_limits<ll>::max();

    for (int i = 1; i < n + 1; i++)
    {
        ans = min(ans, opt[i] + pre_sum[n] - pre_sum[i]);
    }
    cout << ans << endl;

    return 0;
}

void divide_and_conquer(ll l, ll r)
{
    if (l != r)
    {
        ll mid = l + (r - l) / 2;
        divide_and_conquer(l, mid);
        update_right(l, r);
        divide_and_conquer(mid + 1, r);
    }
}

void update_right(ll l, ll r)
{
    ll mid = l + (r - l) / 2;
    for (int i = l - 1; i <= r; i++)
    {
        nodes[i] = node(p[i].y, i);
    }

    sort(nodes.begin() + l - 1, nodes.begin() + r + 1);

    int has_num = 0, pre = -1;
    for (int i = l - 1; i <= r; i++)
    {
        if (nodes[i].y != pre)
        {
            has_num++;
        }
        pre = nodes[i].y;
        map_arr[nodes[i].id] = has_num;
    }

    for (int i = l; i <= mid; i++)
    {
        points[i] = p[i - 1];
        points[i].id = i;
    }

    for (int i = mid + 1; i <= r; i++)
    {
        points[i] = p[i];
        points[i].id = i;
    }

    sort(points.begin() + l, points.begin() + r + 1, low_cmp);

    u_tree.clear(has_num);
    d_tree.clear(has_num);

    for (int i = l; i <= r; i++)
    {
        if (points[i].id <= mid)
        {
            u_tree.update(map_arr[points[i].id - 1], opt[points[i].id] - pre_sum[points[i].id] - points[i].x - points[i].y);
            d_tree.update(map_arr[points[i].id - 1], opt[points[i].id] - pre_sum[points[i].id] - points[i].x + points[i].y);
        }
        else
        {
            ll value = min(u_tree.query(map_arr[points[i].id]) + pre_sum[points[i].id - 1] + points[i].x + points[i].y, d_tree.query(map_arr[points[i].id]) + pre_sum[points[i].id - 1] + points[i].x - points[i].y);
            opt[points[i].id] = min(opt[points[i].id], value);
        }
    }

    u_tree.clear(has_num);
    d_tree.clear(has_num);

    sort(points.begin() + l, points.begin() + r + 1, high_cmp);

    for (int i = l; i <= r; i++)
    {
        if (points[i].id <= mid)
        {
            u_tree.update(map_arr[points[i].id - 1], opt[points[i].id] - pre_sum[points[i].id] + points[i].x - points[i].y);
            d_tree.update(map_arr[points[i].id - 1], opt[points[i].id] - pre_sum[points[i].id] + points[i].x + points[i].y);
        }
        else
        {
            ll value = min(u_tree.query(map_arr[points[i].id]) + pre_sum[points[i].id - 1] - points[i].x + points[i].y, d_tree.query(map_arr[points[i].id]) + pre_sum[points[i].id - 1] - points[i].x - points[i].y);
            opt[points[i].id] = min(opt[points[i].id], value);
        }
    }
}