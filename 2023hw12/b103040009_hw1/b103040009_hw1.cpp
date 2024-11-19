#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void add_choose(int* arr,int arrlength){
    int carry=1;
    for(int i = arrlength;i>0;i--){
        if(arr[i]==1&&carry==1){
            arr[i]=0;
            carry=1;
        }
        else if(arr[i]==0&&carry==1){
            arr[i]=1;
            carry=0;
        }
    }
}
bool finish(int* choose,int numItem){
    int flag=0;
    for(int i=1;i<=numItem;i++){
        if(choose[i]==0){
            flag=1;
            break;
        }
    }
    if(flag==1){
        return false;
    }
    else return true;
}
int main(){
    string folderName,path;
    cout<<"Input folder name:";
    cin>>folderName;
    path = "./dataset/"+folderName+"/item.txt";

    ifstream in;
    in.open(path,ios::in);
    int numItem=0,maxCapacity=0;
    int *itemWeight,*itemValue;

    in>>numItem>>maxCapacity;
    
    itemWeight = new int[numItem+1];
    itemValue = new int[numItem+1];

    for(int i=1;i<=numItem;i++){
        in>>itemWeight[i]>>itemValue[i];
    }
    in.close();
    /////////////////////////////////////////////////////
    //Brute force:
    int* choose;
    choose = new int[numItem+1];
    for(int i=0;i<=numItem;i++){
        choose[i] = 0;
    }

    long long value=0,maximumValue=0,weight=0;
    string solution;

    while(!finish(choose,numItem)){
        weight=0;
        for(int i=1;i<=numItem;i++){
            if(choose[i]==1){
                weight+=itemWeight[i];
            }
        }
        if(weight<=maxCapacity){
            value=0;
            for(int i=1;i<=numItem;i++){
                if(choose[i]==1){
                    value+=itemValue[i];
                }
            }
            if(value>maximumValue){
                maximumValue = value;
                solution = "";
                for(int i=1;i<=numItem;i++){
                    solution+=char(choose[i]+'0');
                }
            }
        }
        add_choose(choose,numItem);

    }

    ////////////////////////////////////////////////////
    ofstream out;
    path = "./dataset/"+folderName+"/ans_"+folderName+".txt";
    out.open(path,ios::out);
    out<<"max profit:"<<maximumValue<<endl;
    out<<"solution:"<<solution;

    cout<<"done"<<endl;

    out.close();

    return 0;
}
