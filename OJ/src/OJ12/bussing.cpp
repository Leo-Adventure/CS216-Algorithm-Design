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
};

ll calculte_dist(Point &p1, Point &p2);

int main()
{
    int num;
    cin >> num;
    vector<Point> points(num);
    for (int i = 0; i < num; ++i)
    {
        ll x, y, z;
        cin >> x >> y >> z;
        Point p(x, y, z);
        points[i] = p;
    }
   

    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(points), std::end(points), rng);

    dist = calculte_dist(points[0], points[1]);

    for (int i = 1; i < num; i++)
    {
        for (int j = i + 1; j < num; j++)
        {
            dist = min(dist, calculte_dist(points[i], points[j]));
        }
    }
    cout << "dist = " << dist << endl;


}

ll calculte_dist(Point &p1, Point &p2)
{
    ll x_dis = pow(p1.x - p2.x, 2);
    ll y_dis = pow(p1.y - p2.y, 2);
    ll z_dis = pow(p1.z - p2.z, 2);
    return (x_dis + y_dis + z_dis);
}