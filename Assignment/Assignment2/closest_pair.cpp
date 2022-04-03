#include<iostream>
#include<vector>
#include<algorithm>
#include<limits>
#include<map>

using namespace std;

// define a Point structure
struct Point{
    size_t x;
    size_t y;
    size_t z;
    Point(): x(0), y(0), z(0){};
};

// comparator to compare x
bool cmp_x(Point p1, Point p2){
    return p1.x < p2.x;
}

// comparator to compare y
bool cmp_z(Point p1, Point p2){
    return p1.z < p2.z;
}

// get distance of two points
double get_distance(Point p1, Point p2);

double Recursively_Get_Closest_Distance(vector<Point>& p_x, vector<Point>& p_z);

double Find_Closest_Pair_distance(vector<Point>& P){
    size_t P_size = P.size();
    vector<Point> P_z(P_size);
    for(size_t i = 0; i < P_size; i++){
        P_z[i] = P[i];
    }

    sort(P.begin(), P.end(), cmp_x); // sort according to the x axis, then get P_x
    sort(P_z.begin(), P_z.end(), cmp_z); // sort according to the z axis, then get P_z
    return Recursively_Get_Closest_Distance(P, P_z);

}

double get_distance(Point p1, Point p2){
    int delta_x = p1.x - p2.x;
    int delta_y = p1.y - p2.y;
    int delta_z = p1.z - p2.z;
    double distance = sqrt(pow(delta_x, 2) + pow(delta_y, 2) + pow(delta_z, 2));
    return distance;
}


double Recursively_Get_Closest_Distance(vector<Point>& p_x, vector<Point>& p_z){
    // if the size of P is no larger than 3, then return the minimum distance directly
    size_t p_size = p_x.size();
    if(p_size <= 3){
        if(p_size == 1){
            // inform the user about the wrong size
            cerr << "The input point set's size should be larger than 1" << endl;
            return -1;
        }else if(p_size == 2){
            return get_distance(p_x[0], p_x[1]);
        }else{
            double d1 = get_distance(p_x[0], p_x[1]);
            double d2 = get_distance(p_x[1], p_x[2]);
            double d3 = get_distance(p_x[0], p_x[2]);
            return min(d1, min(d2, d3));
        }
    }
    
    // the middle point of P
    Point mid_point_x = p_x[p_size / 2];
    size_t mid_x = mid_point_x.x;

    // construct the four arrays according to their position
    vector<Point> Q_x = vector<Point>(p_x.begin(), p_x.begin() + p_size / 2);
    vector<Point> R_x = vector<Point>(p_x.begin() + p_size / 2, p_x.end());
    vector<Point> Q_z;
    vector<Point> R_z;

    // iterate the P_z from small to large, and put the points into Q_z and R_z according to the position in x axis
    for(size_t i = 0; i < p_size; i++){
        Point p = p_z[i];
        if(p.x <= mid_x){
            Q_z.push_back(p);
        }else{
            R_z.push_back(p);
        }
    }
    // call the method recursively
    double dist1 = Recursively_Get_Closest_Distance(Q_x, Q_z);
    double dist2 = Recursively_Get_Closest_Distance(R_x, R_z);
    double dist = min(dist1, dist2); // assign the dist to the minimum of dist1, dist2.

    vector<Point> S1, S2;
    size_t max_y = 0;
    for(size_t i = 0; i < p_size; i++){
        Point p = p_z[i];
        if(abs(int(p.x) - int(mid_x)) < dist && p.x <= mid_x){
            S1.push_back(p);
            if(p.y > max_y){
                max_y = p.y;
            }
        }else if(abs(int(p.x) - int(mid_x)) < dist && p.x > mid_x){
            S2.push_back(p);
            if(p.y > max_y){
                max_y = p.y;
            }
        }
    }

    // declare and initialize the map
    map<size_t, vector<Point>> map;
    size_t block_num = max_y / (2*dist);
    for(size_t i = 0; i < block_num; i++){
        map[i] = vector<Point>(0);
    }

    
    size_t ptr1 = 0, ptr2 = 0;
    size_t s1_size = S1.size();
    size_t s2_size = S2.size();
    // record the reference location of the points of S2
    // so that if I know the point idx of S1, I can find the reference location immediately
    vector<size_t> refer1(s1_size, 0l);
    vector<size_t> refer2(s1_size, 0l);
    while(ptr1 < s1_size && ptr2 < s2_size){
        // if the fetched element is from S1
        if(S1[ptr1].z < S2[ptr2].z){
            Point p = S1[ptr1];
            size_t y = p.y;
            // find the index of field
            if(y / dist == 0){
                refer1[ptr1] = -1;
                refer2[ptr1] = map[0].size();
            }else{
                size_t idx = (y - dist) / (2*dist) + 1;
                refer1[ptr1] = map[idx - 1].size();
                refer2[ptr2] = map[idx].size();
            }
        }else{ //if the fetched element is from S2 
            Point p = S2[ptr2];
            size_t y = p.y;
            // calculate the index of S2
            size_t idx = y / (2 * dist);
            map[idx].push_back(p);
        }
    }

    double min_dist = numeric_limits<double>::max();
    for(size_t i = 0; i < s1_size; i++){
        Point p = S1[i];
        size_t y = p.y;
        size_t idx = 0;
        if(y >= dist){
            idx = y / (2 * dist) + 1;
        }
        size_t ref1 = refer1[idx - 1];
        size_t ref2 = refer2[idx];
        for(size_t i = 0; i < 8; i++){
            double d1 = -1, d2 = -1, d3 = -1, d4 = -1;
            // update the distance if the access is legal
            if(ref1 - i >= 0){
                d1 = get_distance(map[idx - 1][ref1 - i], p);
                min_dist = min(min_dist, d1);
            }
            if(ref2 - i >= 0){
                d2 = get_distance(map[idx][ref2 - i], p);
                min_dist = min(min_dist, d2);
            }
            if(ref1 + i < map[idx - 1].size()){
                d3 = get_distance(map[idx - 1][ref1 + i], p);
                min_dist = min(min_dist, d3);
            }
            if(ref2 + i < map[idx - 1].size()){
                d4 = get_distance(map[idx][ref2 + i], p);
                min_dist = min(min_dist, d4);
            }
        }
        return min(min_dist, dist);
    }
}

