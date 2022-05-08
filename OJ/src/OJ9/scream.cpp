#include <iostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;
using ll = long long;

const ll inf = numeric_limits<int>::max();
string str = "";

void dp(int l, int r, vector<vector<ll>> &opt);

int main()
{
    cin >> str;
    int size = str.size();

    vector<vector<ll>> opt(size, vector<ll>(size, 0));

    for (int i = 0; i < size; i++)
    {
        if (str[i] != '*')
        {
            opt[i][i] = 1;
        }
    }

    for (int i = 1; i < size; i++)
    {
        for(int j = 0; j + i < size; j ++){
            dp(j, j + i, opt);
        }
    }
/*
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (opt[i][j] == inf)
            {
                cout << "inf ";
            }
            else
            {
                cout << opt[i][j] << " ";
            }
        }
        cout << endl;
    }
*/

    if(opt[0][size - 1] < inf){
        cout << opt[0][size - 1] << endl;
    }else{
        cout << "AHHHHH" << endl;
    }
    
    return 0;
}

void dp(int l, int r, vector<vector<ll>> &opt)
{
    if (l >= r)
    {
        return;
    }

    if (str[l] == '*' && str[r] == '*')
    {

        opt[l][r] = min(opt[l + 1][r - 1], min(opt[l + 1][r], opt[l][r - 1]));
    }
    else if (str[l] == '*')
    {
        opt[l][r] = min(opt[l + 1][r], opt[l][r - 1] + 2);
    }
    else if (str[r] == '*')
    {
        opt[l][r] = min(opt[l][r - 1], opt[l + 1][r] + 2);
    }
    else
    {
        if (str[l] == str[r] || str[l] == '?' || str[r] == '?')
        {
            opt[l][r] = opt[l + 1][r - 1] + 2;
        }
        else
        {
            opt[l][r] = inf;
        }
    }
}