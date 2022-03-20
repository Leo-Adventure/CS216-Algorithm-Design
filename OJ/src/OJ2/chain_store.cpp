#include<iostream>
#include<vector>
#include<queue>

using namespace std;

int main(){
    int store_num = 0;
    int open_day = 0;
    int break_day = 0;
    int total_day = 0;

    cin >> store_num >> total_day >> open_day >> break_day;

    priority_queue<pair<int, int>> store_queue; // 按照所需天数降序给店铺排序
    queue<pair<int, int>> cache_queue; // 作为 store_queue 堵塞时候的缓冲队列

    vector<int> date(total_day, 0); // 存储每天所需要开张的店铺要求数量
    vector<int> available(store_num, 0); // 存储各商店最近可以开放的日期
    vector<int> open(store_num, -1); // 存储各商店开放到的日期， 初始化为-1
    vector<int> remain(store_num, 0); // 存储各商店仍然需要开放的日子数量

    for(int i = 0; i < store_num; i++){ // 读取并存储 每一家店铺对应的{剩余需要天数，店铺序号}
        int day;
        cin >> day;
        remain[i] = day;
        store_queue.push({day, i});
    }

    for(int i = 0; i < total_day; i++){ // 读取每一天应开的店铺数量
        cin >> date[i];
    }

    //程序正式开始
    
    for(int i = 0; i < total_day; i++){ // 遍历每一天的需求
        // cout << "i = " << i << endl;
        int require = date[i]; // 要求开张的店铺数量
        int cnt = 0; // 已经开张的店铺数量

        for(int j = 0; j < store_num; j++){
            if(open[j] >= i){// 如果开放到的天数在当天之后，开张的店铺数量 cnt 就加一
                cnt ++;
            }
        }

        while(cnt < require && !store_queue.empty()){
            pair<int, int> store = store_queue.top();
            store_queue.pop();
            int remain_day = store.first;
            int id = store.second;
            if(open[id] < i){ // 当前需求天数最多的店铺在这一天没开
                if(available[id] <= i){ // 当前需求天数最多的店铺在这一天可以开，那就开吧
                    cache_queue.push({remain_day - open_day, id}); // 放进缓冲区队列
                    remain[id] -= open_day;
                    cnt ++; // 开张店铺数 + 1
                    available[id] = i + open_day + break_day; //更新该商店最近可开日期
                    open[id] = i + open_day - 1; // 更新该商店开放到的日期
                }else{ // 当前需求天数最多的店铺在这一天不能开
                    cache_queue.push({remain_day, id});
                }
            }else{ // 当前需求天数最多的店铺在这一天已经开了
                cache_queue.push({remain_day, id}); // 放进缓冲区队列
            }
        }

        if(cnt > require){
            cout << "No";
            return 0;
        }

        if(store_queue.empty() && cnt < require){
            cout << "No";
            return 0;
        }

        while(!cache_queue.empty()){
            pair<int, int> p = cache_queue.front();
            cache_queue.pop();
            int remain_day = p.first;
            int id = p.second;
            if(remain_day > 0){ // queue元素非零才重新push回去
                store_queue.push({remain_day, id});
            }
        }
        /*
        cout << "i = " << i << endl;
        cout << "available array: \n";
        for(int i = 0; i < store_num; i++){
            cout << available[i] << " ";
        }
        cout << endl;

        cout << "open array: \n";
        for(int i = 0; i < store_num; i++){
            cout << open[i] << " ";
        }
        cout << endl;
        */
    }
    

    for(int i = 0; i < store_num; i++){
        if(remain[i] != 0){
            cout << "No";
            return 0;
        }
    }

    for(int i = 0; i < store_num; i++){
        if(open[i] >= total_day){
            cout << "No";
            return 0;
        }
        
    }

    cout << "Yes";
    return 0;
}