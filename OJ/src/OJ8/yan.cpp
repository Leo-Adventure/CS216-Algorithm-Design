#include<iostream>
#include<vector>

using namespace std;
typedef long long ll;

vector<vector<ll>> arr;

void construct(ll i);

int main(){
    arr.resize(305);
    arr[0].resize(1);
    arr[1].resize(1);
    arr[0][0] = 1;
    arr[1][0] = 1;
    
    for(int i = 2; i < 305; i ++){
        construct(i);
    }
    ll T;
    cin >> T;
    for(int t = 0; t < T; t++){
        int num;
        cin >> num;
        for(int i = 0; i < num * num; i++){
            if(arr[num][i]){
                cout << i << " ";
            }
        }
        cout << "\n";
    }
}
void construct(ll num){
    
    arr[num].resize(num * num);
    for(int i = 0; i < num; i++){
        ll con = (num - i) * i;
        if(i == 0){
            arr[num][0] = 1;
        }
        for(int j = 0; j < i * i; j ++){
            if(arr[i][j]){
                arr[num][con + j] = 1;
            }
            
        }
    }
}