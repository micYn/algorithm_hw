#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main()
{
    string folderName, path;
    cout << "Input folder name:";
    cin >> folderName;
    path = "./dataset/" + folderName + "/item.txt";

    ifstream in;
    in.open(path, ios::in);
    int numItem = 0, maxCapacity = 0;
    int *itemWeight, *itemValue;

    in >> numItem >> maxCapacity;

    itemWeight = new int[numItem + 1];
    itemValue = new int[numItem + 1];

    for (int i = 1; i <= numItem; i++)
    {
        in >> itemWeight[i] >> itemValue[i];
    }
    in.close();
    //////////////////////////////////////////////////
    // DP:
    // DP[i][j]是前i個物品、背包上限為j時能有的最大價值
    int** DP;
    DP = new int *[numItem + 5];
    for (int i = 0; i < numItem + 5; i++){
        DP[i] = new int[maxCapacity + 5];
    }
    for (int i = 0; i < numItem + 5; i++){
        for (int j = 0; j < maxCapacity + 5; j++){
            DP[i][j] = 0;
        }
    }
    for (int i = 1; i <= maxCapacity; i++){
        for (int j = 1; j <= numItem; j++){
            if (i - itemWeight[j] < 0){
                DP[j][i] = DP[j - 1][i];
            }
            else{
                if(DP[j - 1][i - itemWeight[j]] + itemValue[j] >= DP[j - 1][i]){
                    DP[j][i] = DP[j - 1][i - itemWeight[j]] + itemValue[j];
                }
                else{
                    DP[j][i] = DP[j - 1][i];
                }
            }
        }
    }
    //回推物品拿取的狀況:
    vector<int> ans;
    for (int i = numItem, j = maxCapacity; i >= 1; --i){
        if (j - itemWeight[i] >= 0 && DP[i][j] == DP[i-1][j - itemWeight[i]] + itemValue[i])
        {
            //這代表背包裡面有第i個物品
            ans.push_back(1);
            j -= itemWeight[i];
        }
        else{
            ans.push_back(0);
        }
        
    }
    ///////////////////////////////////////////////////////
    fstream out;
    path = "./dataset/"+folderName+"/ans_"+folderName+".txt";
    out.open(path,ios::out);
    out<<"max profit:"<<DP[numItem][maxCapacity]<<endl;
    out<<"solution:";
    for(int i=ans.size()-1;i>=0;i--){
        out<<ans[i];
    }

    cout<<"done"<<endl;

    out.close();
    for (int i = 0; i < numItem + 5; i++)
    {
        free(DP[i]);
    }
    free(DP);
}