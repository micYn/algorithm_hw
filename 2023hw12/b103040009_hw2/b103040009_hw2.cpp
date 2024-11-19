#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

struct item{
        double itemWeight;
        double itemValue;
        double itemCP;
        int originalindex;
};

bool mycompare(item i1,item i2){
    return i2.itemCP<i1.itemCP;
}

int main(){
    string folderName,path;
    cout<<"Input folder name:";
    cin>>folderName;
    path = "./dataset/"+folderName+"/item.txt";

    ifstream in;
    in.open(path,ios::in);
    int numItem=0,maxCapacity=0;

    item *itemArr;

    in>>numItem>>maxCapacity;
    
    itemArr =  new item[numItem+1];

    for(int i=1;i<=numItem;i++){
        in>>itemArr[i].itemWeight>>itemArr[i].itemValue;
        itemArr[i].itemCP = itemArr[i].itemValue/itemArr[i].itemWeight;
        itemArr[i].originalindex = i;
    }
    in.close();

    //////////////////////////////////////////////////
    //Greedy:
    long long weight=0,value=0;
    char* solution;
    solution = new char[numItem+1];
    for(int i=1;i<=numItem;i++){
        solution[i]='0';
    }
    sort(itemArr+1,itemArr+numItem+1,mycompare);
    
    for(int i=1;i<=numItem;i++){
        //cout<<itemArr[i].itemWeight<<" "<<itemArr[i].itemValue<<" "<<itemArr[i].itemCP<<" "<<itemArr[i].originalindex<<endl;
        if(itemArr[i].itemWeight+weight <= maxCapacity){
            weight += itemArr[i].itemWeight;
            value += itemArr[i].itemValue;
            solution[itemArr[i].originalindex] = '1';
        }
    }
    /////////////////////////////////////////////////////

    ofstream out;
    path = "./dataset/"+folderName+"/ans_"+folderName+".txt";
    out.open(path,ios::out);
    out<<"max profit:"<<value<<endl;
    out<<"solution:";
    for(int i=1;i<=numItem;i++){
        out<<solution[i];
    }

    cout<<"done"<<endl;

    out.close();

    return 0;
}