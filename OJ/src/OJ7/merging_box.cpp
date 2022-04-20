#include<iostream>
#include<vector>
#include <optional>
#include <numeric> 
#include <map> 

using namespace std;

typedef long long ll;

ll merge(vector<ll> nums);

int main(){
    ll N;
    cin >> N;
    vector<ll>nums(N);
    for(int i = 0; i < N; i++){
        cin >> nums[i];
    }
    cout << merge(nums);
    return 0;
}

ll merge(vector<ll> nums){

    static map<vector<ll>, ll> map; 

    {
        // auto f = map.find(nums); 
        // if (f != map.end()) {
        //     return f->second; 
        // }
        if(map.count(nums)){
            return map[nums];
        }

    }

    int size = nums.size();
    if(size == 1){
        return 0; 
    }

    optional<ll> res = nullopt; 

    for(int i = 1; i < size; i++){
        vector<ll> left = vector<ll>(nums.begin(), nums.begin() + i);
        vector<ll> right = vector<ll>(nums.begin() + i, nums.end());
        // cout << "left.size() = " << left.size() << " and right.size() = " << right.size() << endl;
        ll tmp = merge(left) + merge(right);
        if (!res || res > tmp) 
            res = tmp; 
    }
    auto _tmp = *res + accumulate(nums.begin(), nums.end(), 0LL, [](auto a, auto b) {return a + b;
    static_assert(is_same_v<decay_t<decltype(a)>, ll>); });
    
    map[nums] = _tmp; 
    return _tmp; 
}

