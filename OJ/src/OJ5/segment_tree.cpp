#include<iostream>
#include<vector>

using namespace std;

typedef long long ll;

struct Node{
    ll left;
    ll right;
    ll sum;
    ll lazy;
    Node(): left(0), right(0), sum(0), lazy(0){};
};

vector<Node*> nodes;

void update(ll idx){
    // cout << "idx = " << idx << endl;
    nodes[idx] -> sum = nodes[idx * 2] -> sum + nodes[idx * 2 + 1] -> sum;
    // cout << "The idx sum is " << nodes[idx] -> sum << endl;
    // cout << "Out of update" << endl;
}

void build(ll l, ll r, ll idx, vector<ll>& nums){
    // cout << "In build" << endl;
    // cout << "idx = " << idx << endl;
    nodes[idx] -> left = l;
    nodes[idx] -> right = r;
    if(l == r){
        nodes[idx] -> sum = nums[l];
        // cout << "idx->sum = " << nodes[idx] -> sum << endl;
        return;
    }
    ll mid = l + ((r - l) >> 1);//2
    // cout << "mid = " << mid <<" idx = " << idx << " l = " << l << " r = " << r << endl;
    build(l, mid, idx * 2, nums);
    build(mid + 1, r, idx * 2 + 1, nums);
    update(idx);
    // cout << "Out of build" << endl;
}

void press_down(ll idx){
    if(nodes[idx]->lazy == 0){
        return;
    }
    ll left = idx * 2;
    ll right = left + 1;
    nodes[left]->lazy += nodes[idx]->lazy;
    nodes[right]->lazy += nodes[idx]->lazy;
    nodes[left]->sum += nodes[idx]->lazy * (nodes[left]->right - nodes[left]->left + 1);
    nodes[right]->sum += nodes[idx]->lazy * (nodes[right]->right - nodes[right]->left + 1);
    nodes[idx]->lazy = 0;
}


void addition(ll l, ll r, ll val, ll idx){
    if(nodes[idx]->left >= l && nodes[idx] -> right <= r){
        nodes[idx]->sum += val * (nodes[idx]->right - nodes[idx]->left + 1);
        nodes[idx]->lazy += val;
        return;
    }
    if(nodes[idx]->left > r || nodes[idx]->right < l){
        return;
    }
    press_down(idx);
    addition(l, r, val, idx * 2);//1 2 2 2
    addition(l, r, val, idx * 2 + 1);// 1 2 2 3
    update(idx);
}

void print(){
    for(int i = 1; i < nodes.size(); i++){
        cout << "nodes[" << i << "]" << "-> left = " << nodes[i]->left << ", right = " << nodes[i]->right << ", sum = " << nodes[i]->sum << endl;
    }
}

ll query(ll l, ll r, ll idx){ // 1, 3, 1
    ll result = 0;
    if(nodes[idx]->left >= l && nodes[idx]->right <= r){
        return nodes[idx]->sum;
    }
    if(nodes[idx]->right < l || nodes[idx]->left > r){
        
        return 0ll;
    }
    press_down(idx);
    
    result += query(l, r, idx * 2);// 1, 1, 2 // 1, 1, 4
    result += query(l, r, idx * 2 + 1);//2, 3, 3 // 1, 1, 3
    
    return result;
}


int main(){
    ll number_num, op_num;
    cin >> number_num >> op_num;
    vector<ll> nums(number_num, 0);//5
    nodes.resize(number_num * 4 + 1);//21
    for(int i = 0; i < number_num * 4; i++){
        if(i < number_num){
            cin >> nums[i];
        }
        
        nodes[i + 1] = new Node();
    }
    
   
    build(0, number_num - 1, 1, nums);//0, 4, 1, nums
    // cout << "out" << endl;
    

    for(int i = 0; i < op_num; i++){
        int op;
        cin >> op;
        if(op == 1){
            int left, right, val;
            cin >> left >> right >> val;
            addition(left - 1, right - 1, val, 1);
        }else{
            int left, right;
            cin >> left >> right;
            cout << query(left - 1, right - 1, 1) << endl;
        }
        // print();
    }
    return 0;

}