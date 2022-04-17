#include <stdio.h>
#include <stdlib.h> /* 亂數相關函數 */
#include<iostream>
#include <fstream>
#include <time.h>   /* 時間相關函數 */
using namespace std;
int main()
{
    
    srand(time(NULL));
    ofstream fout("bound.txt");
    for (int i = 0; i < 237884; i++)
    {
        int x = rand();
        // printf("x = %d\n", x);
        x %= 25;
        char c = x + 'a';
        // cout << c << endl;
        char* ch = new char[1];
        ch[0] = c;
        fout.write(ch, sizeof(char) * 1);
        delete(ch);
    }
    fout.close();

    return 0;
}