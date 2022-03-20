#include<queue>
#include<iostream>
#include<string>
#include<map>
#include<vector>

using namespace std;

int main(){
    int size = 0;
    scanf("%d", &size);
    //map 根据编号寻找对应字符串（人名）
    map<int, string> map_man;
    map<int, string> map_woman;
    // remap 根据字符串（人名）找对应编号
    map<string, int> remap_man;
    map<string, int> remap_woman;
    //两个二维数组用来存储男生女生的偏好列表
    vector<vector<string>> man(size, vector<string>(size));
    vector<vector<string>> woman(size, vector<string>(size));
    //queue 存储单身男生的队列
    queue<int> que;

    //pre_idx 存储的是各个男生最喜欢的未求婚女生的序号，初始化为0，即每个男生都从最喜欢的女生开始求婚
    vector<int> pre_idx(size);
    for(int i = 0; i < size; i++){
        pre_idx[i] = 0;
    }

    //表示女生当前是否单身
    vector<bool> single(size, 0);

    //pair_map 用于存储当前已经结对的情侣
    map<int, int> pair_man;
    map<int, int> pair_woman;

    for(int i = 0; i < size; i++){
        string man_name;
        cin >> man_name;
        map_man[i] = man_name;
        remap_man[man_name] = i;
        que.push(i);
    }
    
    
    for(int i = 0; i < size; i++){
        string woman_name;
        cin >> woman_name;
        map_woman[i] = woman_name;
        remap_woman[woman_name] = i;
    }

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            cin >> man[i][j];
        }
    }

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            cin >> woman[i][j];
        }
    }


    while(!que.empty()){

        //找出队首的单身男生
        int man_idx = que.front();//1
        que.pop();

        int idx = pre_idx[man_idx]; //找出当前最喜欢的女生编号
        pre_idx[man_idx]++;
        

        string name = man[man_idx][idx];
        int woman_idx = remap_woman[name];// 通过编号找到对应的女生
        
        if(!single[woman_idx]){//仍然单身则直接结对
            pair_man[man_idx] = woman_idx;
            pair_woman[woman_idx] = man_idx;
            single[woman_idx] = 1;//修改状态
            // cout << "now " << map_man[man_idx] << " " << map_woman[woman_idx] << endl;
        }else{
            int exi_idx = pair_woman[woman_idx]; //现任男友编号 //2
            string want_marry_man = map_man[man_idx];//B
            string curr_marry_man = map_man[exi_idx];//C
            // cout << "want_marry_man = " << want_marry_man << " curr_marry_man = " << curr_marry_man << endl;
            for(int i = 0; i < size; i++){
                // cout << "The woman is " << map_woman[woman_idx] << endl;
                if(woman[woman_idx][i] == want_marry_man){
                    // cout << "i = " << i << endl;
                    // cout << "Strange " << woman[woman_idx][i] << " and " << want_marry_man << endl;
                    que.push(exi_idx);
                    // cout << "now push " << exi_idx << endl;
                    pair_man[man_idx] = woman_idx;
                    pair_woman[woman_idx] = man_idx;
                    // cout << "now " << want_marry_man << " " << map_woman[woman_idx] << endl;
                    break;
                }

                if(woman[woman_idx][i] == curr_marry_man){
                    que.push(man_idx);
                    // cout << "now push " << man_idx << endl;
                    break;
                }
            }
        }
    }

    for(int i = 0; i < size; i++){
        cout << map_man[i] << " " << map_woman[pair_man[i]] << endl;
    }
    return 0;
}