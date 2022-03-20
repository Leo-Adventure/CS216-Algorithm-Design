#include<iostream>
#include<vector>
#include<queue>
using namespace std;

int main(){
    // que 存储当前没有学校读并且依然存在候选学校评分大于零的学校的学生
    queue<int> que;

    int stu_num = 0; int col_num = 0;
    cin >> stu_num >> col_num;// 读取学生以及学校的数量

    //cap 存储每个学校的最大容量
    vector<int> cap(col_num);
    for(int i = 0; i < col_num; i++){
        cin >> cap[i]; // 读取每个学校分别的最大学生容量
    }
    //curr 存储每个学校当前容纳的学生数量，初始化为 0
    vector<int> curr(col_num, 0);

    // 分别创建两个数组 分别存储学校以及学生的偏好分数
    vector<vector<int>> college(col_num, vector<int>(stu_num));
    vector<vector<int>> student(stu_num, vector<int>(col_num));

    for(int i = 0; i < stu_num; i++){

        que.push(i);//将学生编号逐个放入队列当中

        for(int j = 0; j < col_num; j++){// 读取各个学生对学校的评分
            cin >> student[i][j];
        }

    }

    for(int i = 0; i < col_num; i++){
        for(int j = 0; j < stu_num; j++){// 读取各个学校对学生的评分
            cin >> college[i][j];
        }
    }

    // 用于存放匹配结果的数组
    vector<vector<int>> res(col_num, vector<int>(0));

    while(!que.empty()){// O(stu_num) complexity

        int stu_idx = que.front();
        que.pop();

        // 选择分数最高的学校将要就读
        int col_idx = 0;
        int max_pt = -1;
        // O(col_num) complexity
        for(int i = 0; i < col_num; i++){
            if(student[stu_idx][i] > max_pt){
                max_pt = student[stu_idx][i];
                col_idx = i;
            }
        }

        if(max_pt < 0){
            continue;
        }else{
            student[stu_idx][col_idx] = -1;
        }

        if(curr[col_idx] < cap[col_idx]){//名额未满
            if(college[col_idx][stu_idx] >= 0){//评分大于零才接收该学生
                res[col_idx].push_back(stu_idx);
                curr[col_idx]++;
            }else{
                que.push(stu_idx);//否则学生继续没有学校上
            }
        }else{// 名额已满
            if(college[col_idx][stu_idx] <= 0){
                que.push(stu_idx);
            }else{
                //挑出当前最少评分的学生与待进入学生进行比较
                int min_pt = college[col_idx][res[col_idx][0]]; // 结果数组当中首个学生对应的分数
                int min_idx = 0;
                for(int i = 1; i < cap[col_idx]; i++){
                    if(college[col_idx][res[col_idx][i]] < min_pt){
                        min_idx = i;
                        min_pt = college[col_idx][res[col_idx][i]];
                    }
                }

                int cur_pt = college[col_idx][stu_idx]; //当前学生的分数
                if(cur_pt > min_pt){
                    que.push(res[col_idx][min_idx]);
                    res[col_idx][min_idx] = stu_idx;
                }else{
                    que.push(stu_idx);
                }
            }
        }
    }
    
    // 输出结果
    for(int i = 0; i < col_num; i++){
        int s = res[i].size();
        cout << s << " ";
        for(int j = 0; j < s; j++){
            cout << res[i][j] + 1 << " ";
        }
        cout << endl;
    }
    return 0;
}
