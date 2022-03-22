#include <iostream>
#include <string>
#include <map>

using namespace std;

struct Node
{
    int val; // 该节点代表的键值
    int key;
    int location; // 0 代表在 young 区， 1 代表在 old 区
    Node *prev;
    Node *next;
    Node() : key(0), val(0), location(0), prev(nullptr), next(nullptr){};
};

int main()
{
    int young_size, old_size;
    cin >> young_size >> old_size;
    int K = 0;
    cin >> K;

    map<int, Node *> m; //哈希链表
    // young 区以及 old 区当前大小
    int young_cur = 0;
    int old_cur = 0;
    // young 区以及 old 区的区首区尾标识指针
    Node *young_head = nullptr;
    Node *young_tail = nullptr;
    Node *old_head = nullptr;
    Node *old_tail = nullptr;

    for (int k = 0; k < K; k++)
    {
        string operation;
        cin >> operation;
        if (operation == "put" || operation == "get")
        {
            // cout << "In line " << __LINE__ << "operation is " << operation << endl;
            int key, value;
            cin >> key;
            if (operation == "put")
            {
                cin >> value;
            }

            if (!m.count(key)) //该值是新的，不存在
            {
                if (operation == "get")
                {
                    cout << "-1\n";
                    continue;
                }

                // 新建节点并且将哈希链表指向该节点
                Node *node = new Node();
                m[key] = node;
                node->key = key;
                node->val = value;

                if (young_cur != young_size)
                { // young 区没有满
                    if (!young_head)
                    { // 当前young区没有元素
                        young_head = node;
                        young_tail = node;
                        // cout << "In" << __LINE__ << ": young_head -> key = " << young_head -> key<< " val = " << young_head -> val << endl;
                        young_cur++;
                    }

                    else // 当前young区有元素
                    {
                        node->next = young_head;
                        young_head->prev = node;
                        young_head = node;
                        node->prev = nullptr;
                        young_cur++;
                    }
                    node->location = 0;
                }
                else
                { // young 区已经满了
                    if (!old_head)
                    { // old 区没有元素
                        old_head = node;
                        old_tail = node;
                        old_head->prev = young_tail;
                        young_tail->next = old_head;
                        old_cur++;
                    }
                    else
                    { // old 区有元素
                        node->next = old_head;
                        node->prev = young_tail;
                        old_head->prev = node;
                        old_head = node;
                        old_cur++;
                        if (old_cur > old_size)
                        { // old区爆掉了
                            m.erase(old_tail->key);
                            old_tail = old_tail->prev;
                            delete (old_tail->next); // 回收内存
                            old_tail->next = nullptr;
                            old_cur--;
                        }
                    }
                    node->location = 1;
                }
            }
            else //值已经存在
            {

                if (operation == "get")
                {
                    cout << m[key]->val << endl;
                }

                Node *node = m[key];

                if (operation == "put")
                {
                    node->val = value;
                }

                if (node->location == 0)
                { // 在young区，不会溢出
                    if (young_head != nullptr && node != nullptr && young_head->key == node->key)
                    {
                        // cout << "Exist in head!!!\n";
                    }
                    else if (young_tail != nullptr && node != nullptr && young_tail->key == node->key)
                    {
                        young_tail = young_tail->prev;
                        young_tail->next = old_head;

                        node->next = young_head;
                        young_head->prev = node;
                        young_head = node;
                        node->prev = nullptr;
                    }
                    else
                    {
                        if (node != nullptr && node->prev != nullptr)
                        {
                            node->prev->next = node->next;
                        }
                        if (node != nullptr && node->next != nullptr)
                        {
                            node->next->prev = node->prev;
                        }

                        node->next = young_head;
                        young_head->prev = node;
                        young_head = node;
                        node->prev = nullptr;
                    }
                }
                else
                { // 在old 区
                    if (old_head->key == node->key)
                    {
                        
                        if (old_head != nullptr && old_head->next != nullptr)
                        {
                            old_head->next->prev = young_tail;
                        }

                        young_tail->next = old_head->next;

                        old_head = young_tail;
                        old_head -> location = 1;

                        young_tail = young_tail->prev;

                        node->next = young_head;
                        young_head->prev = node;
                        young_head = node;
                        node->prev = nullptr;
                        
                    }
                    else if (old_tail->key == node->key)
                    {
                         
                        old_tail = old_tail->prev;
                        old_tail->next = nullptr;

                        old_head->prev = young_tail;
                        young_tail->next = old_head;

                        old_head = old_head->prev;
                        old_head -> location = 1;
                        young_tail = young_tail->prev;

                        node->next = young_head;
                        young_head->prev = node;
                        young_head = node;
                        node->prev = nullptr;
                    }
                    else
                    {
                        // cout << "in line " << __LINE__ <<endl;
                        if (node != nullptr && node->next != nullptr)
                        {
                            node->next->prev = node->prev;
                        }
                        if (node != nullptr && node->prev!= nullptr)
                        {
                            node->prev->next = node->next;
                        }

                        old_head->prev = young_tail;
                        young_tail->next = old_head;

                        young_tail = young_tail->prev;
                        old_head = old_head->prev;

                        old_head -> location = 1;

                        node->next = young_head;
                        young_head->prev = node;
                        young_head = node;
                        
                        node->prev = nullptr;
                        
                    }
                }
                node->location = 0;
            }
        }
        else if (operation == "print")
        {
            // 遍历输出
            Node *node = young_head;
            // cout << "In " << __LINE__ << ": node -> key = " << node -> key<< ", val = " << node -> val << endl;
            for (int i = 0; i < young_cur && node != nullptr; i++)
            {
                cout << node->key << ":" << node->val << " ";
                node = node->next;
            }
            node = old_head;
            for (int i = 0; i < old_cur && node != nullptr; i++)
            {
                cout << node->key << ":" << node->val << " ";
                node = node->next;
            }
            printf("\n");
        }
       
    }
    return 0;
}
