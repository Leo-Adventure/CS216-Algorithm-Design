#include<iostream>
#include<vector>
#include<algorithm>

typedef long long ll;
using namespace std;

int main(){
    
    int village_num = 0;
    int edge_num = 0;
    cin >> village_num >> edge_num;
    vector<long long> points(village_num, 0);
    for(int i = 0; i < village_num; i++){
        long long num;
        cin >> num;
        points[i] = num * 2;
    }

    for(int i = 0; i < edge_num; i++){
        int first = 0;
        int second = 0;
        long long b = 0;
        cin >> first >> second >> b;
        points[first - 1] += b;
        points[second - 1] += b;
    }

    sort(points.begin(), points.end(), greater<long long>());
   

    long long p1 = 0ll;
    long long p2 = 0ll;
    for(int i = 0; i < village_num; i++){
        if(i % 2 == 0){
            p1 += points[i];
        }else{
            p2 += points[i];
        }
    }
    long long res = p1 - p2;
    long long r = res / 2;
    cout << r << endl;
    
    return 0;

}