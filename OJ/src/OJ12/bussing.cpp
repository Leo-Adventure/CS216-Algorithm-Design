#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
using ll = long long;

ll dist = numeric_limits<ll>::max();

struct Point
{
    ll x;
    ll y;
    ll z;
    Point() : x(0), y(0), z(0){};
    Point(ll in_x, ll in_y, ll in_z) : x(in_x), y(in_y), z(in_z){};
    bool operator == (const Point &rhs) const noexcept {
        return x == rhs.x && y == rhs.y && z == rhs.z; 
    }
};

struct cmp
{
    int operator()(tuple<ll, ll, ll> i) const
    {
        ll con = 10000;
        return get<0>(i) + con * get<1>(i) + con * con * get<2>(i);
    }
};

struct point_cmp{
    int operator()(Point t) const{
        ll con = 10000;
        return t.x + con * t.y + con * con * t.z;
    }
};

unordered_map<tuple<ll, ll, ll>, ll, cmp> map;

ll calculte_dist(Point &p1, Point &p2);

ll make_dictionary(vector<Point> &points, ll temp_dist)
{
    int size = points.size();
    ll delta = sqrt(temp_dist) / 2;
    map.clear();
    for (int i = 0; i < size; i++)
    {
        Point p = points[i];

        ll xid = p.x / delta;
        ll yid = p.y / delta;
        ll zid = p.z / delta;

        if (map.count({xid, yid, zid}))
        {
            return calculte_dist(p, points[map[{xid, yid, zid}]]);
        }

        ll minimum = temp_dist;
        for (ll x = xid - 2; x < xid + 3; x++)
        {
            for (ll y = yid - 2; y < yid + 3; y++)
            {
                for (ll z = zid - 2; z < zid + 3; z++)
                {
                    if (x >= 0 && y >= 0 && z >= 0)
                    {
                        if (map.count({x, y, z}))
                        {
                            ll res = calculte_dist(p, points[map[{x, y, z}]]);
                            if(minimum > res)
                                minimum = res;
                        }
                    }
                }
            }
        }

        if (minimum < temp_dist)
        {
            return minimum;
        }
        else
        {
            map[{xid, yid, zid}] = i;
        }
    }
    return -1;
}

int main()
{
    int num;
    cin >> num;
    vector<Point> points(num);
    unordered_map<Point, ll, point_cmp>point_map;
    for (int i = 0; i < num; ++i)
    {
        ll x, y, z;
        cin >> x >> y >> z;
        Point p(x, y, z);
        points[i] = p;
        if(point_map.count(p)){
            cout << 0 << endl;
            return 0;
        }else{
            point_map[p] = i;
        }
    }

    // auto rng = std::default_random_engine{};
    std::shuffle(std::begin(points), std::end(points), std::minstd_rand0());

    dist = calculte_dist(points[0], points[1]);

    while (true)
    {
        ll flag = make_dictionary(points, dist);
        if (flag == -1)
        {
            cout << dist << endl;
            return 0;
        }
        else
        {
            dist = flag;
        }

        // cout << "dist = " << dist << endl;
    }
}

ll calculte_dist(Point &p1, Point &p2)
{
    ll x_dis = pow(p1.x - p2.x, 2);
    ll y_dis = pow(p1.y - p2.y, 2);
    ll z_dis = pow(p1.z - p2.z, 2);
    return (x_dis + y_dis + z_dis);
}