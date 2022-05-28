#include <iostream>
#include<vector>
using namespace std;
using ll = long long;
int main()
{
    string a, b;
    cin >> a >> b;
    ll a_size = a.size();
    ll b_size = b.size();
    ll cnt = 0;
    ll ptr = 0;
    vector<ll>position;
    for(int i = 0; i < a_size; i++){
        // cout << "i = " << i << endl;
        while(ptr < b_size && i + ptr < a_size && a[i + ptr] == b[ptr] || (b[ptr] != 'z' && a[i + ptr] == char(b[ptr] + 1))){
            ptr++;
            // cout << char(a[i + ptr]) << " " << char(b[ptr] + 1) << endl;
            // cout << "ptr = " << ptr << endl;
        }
        if(ptr == b_size){
            // cout << "in\n";
            cnt++;
            position.emplace_back(i + 1);
        }
        ptr = 0;
    }
    ll pos_size = position.size();
    cout << cnt << "\n";
    for(int i = 0; i < pos_size; i++){
        cout << position[i] << " ";
    }
    cout << "\n";
}