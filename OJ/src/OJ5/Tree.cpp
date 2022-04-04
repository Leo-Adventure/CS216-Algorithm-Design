#include<iostream>
#include<vector>

using namespace std;

typedef long long ll;

// The structure of a segment_tree
struct Node{
    ll left;
    ll right;
    ll sum; // the sum of interval [left, right]
    ll lazy; // lazy tag, to delay the assignment in order to speed up
    Node(): left(0), right(0), sum(0), lazy(0){};
};

vector<Node*> nodes;

// update the father node's sum, which equals to the addition of two children's sum.
void update(ll idx){
    nodes[idx] -> sum = nodes[idx * 2] -> sum + nodes[idx * 2 + 1] -> sum;
}

// construct a segment tree using the array.
// O(logn)
void build(ll l, ll r, ll idx, vector<ll>& nums){
    nodes[idx] -> left = l;
    nodes[idx] -> right = r;
    // when the interval only contains one element, the sum is equal to the element.
    if(l == r){
        nodes[idx] -> sum = nums[l];
        return;
    }
    // build children node recursively.
    ll mid = l + ((r - l) >> 1);//2
    build(l, mid, idx * 2, nums);
    build(mid + 1, r, idx * 2 + 1, nums);
    // update the sum of father at last
    update(idx);
}

// press the lazy tag down
void press_down(ll idx){
    if(nodes[idx]->lazy == 0){
        return;
    }
    // if the tag is not zero, then press it down.
    ll left = idx * 2;
    ll right = left + 1;
    nodes[left]->lazy = nodes[idx]->lazy;
    nodes[right]->lazy = nodes[idx]->lazy;
    // update the sum owing to the change of its child.
    nodes[left]->sum = nodes[idx]->lazy * (nodes[left]->right - nodes[left]->left + 1);
    nodes[right]->sum = nodes[idx]->lazy * (nodes[right]->right - nodes[right]->left + 1);
    nodes[idx]->lazy = 0;
}

// set the lazy tag to the new value.
void setValue(ll l, ll r, ll val, ll idx){
    // if the node's interval is included by the whole interval, then just set the lazy tag
    //no need to press it down.
    if(nodes[idx]->left >= l && nodes[idx] -> right <= r){
        nodes[idx]->sum = val * (nodes[idx]->right - nodes[idx]->left + 1);
        nodes[idx]->lazy = val;
        return;
    }
    if(nodes[idx]->left > r || nodes[idx]->right < l){
        return;
    }
    press_down(idx);
    setValue(l, r, val, idx * 2);//1 2 2 2
    setValue(l, r, val, idx * 2 + 1);// 1 2 2 3
    update(idx);
}


// print the detailed information of the segment tree
void print(){
    for(int i = 1; i < nodes.size(); i++){
        cout << "nodes[" << i << "]" << " -> left = " << nodes[i]->left << ", right = " << nodes[i]->right << ", sum = " << nodes[i]->sum << endl;
    }
}

// query for the interval sum
ll query(ll l, ll r, ll idx){ // 1, 3, 1
    ll result = 0;
    if(nodes[idx]->left >= l && nodes[idx]->right <= r){
        return nodes[idx]->sum;
    }
    if(nodes[idx]->right < l || nodes[idx]->left > r){
        
        return 0ll;
    }
    press_down(idx);
    
    result += query(l, r, idx * 2);
    result += query(l, r, idx * 2 + 1);
    
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
    // first build the segment tree
    build(0, number_num - 1, 1, nums);//0, 4, 1, nums

    for(int i = 0; i < op_num; i++){
        int op;
        cin >> op;
        if(op == 1){
            int x, y;
            cin >> x >> y;
            setValue(x - 1, x - 1, y, 1);
        }else{
            int left, right;
            cin >> left >> right;
            cout << query(left - 1, right - 1, 1) << endl;
        }
        // print();
    }
    return 0;

}