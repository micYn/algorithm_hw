/*
    dt01: 200,0.9,0.01 | 1000
    dt02: 250,0.9,0.1 | 10000
    dt03: 75,0.85,0.55 | 1000000 (10min)
    dt03加分: 100,0.85,0.55 | 500000 (10min)  
*/
#include <iostream>
#include <fstream>
#include <time.h>
#include <iomanip>
using namespace std;
const int SIZE = 100;
const double crossover_rate = 0.85;
const double mutation_rate = 0.55;

struct cell{
    int value;
    int weight;
};
void selection(int*** chromosomes,int index, cell* cells, int maxCapacity, int numItem){
    //selection會先檢查chromosomes[index]有沒有人超出背包承重，確保沒有人超重後，
    //計算每個人的value，(挑一個value最大的一定會在)，剩下的隨機挑size-1個(也是依照value大小).丟到chromosomes[(index+1)%2][][]

    /*檢查並修正超出最大承重的解，順便算他的value*/
    int totalWeight[SIZE];
    int totalValue[SIZE];
    for(int l=0;l<SIZE;l++){
        totalWeight[l]=0;
        totalValue[l]=0;
        for(int j=0;j<numItem;j++){
            totalWeight[l] += chromosomes[index][l][j]*cells[j].weight;
            totalValue[l] += chromosomes[index][l][j]*cells[j].value;
        }
        while(totalWeight[l]>maxCapacity){
            int tmp = rand()%numItem;
            if(chromosomes[index][l][tmp]==1){
                chromosomes[index][l][tmp]=0;
                totalWeight[l]-=cells[tmp].weight;
                totalValue[l]-=cells[tmp].value;
            }
        }
    }
    /*******************************************************/
    /*這部分先丟了最棒的解到chromosomes[(index+1)%2][][]*/
    // int max_index = -1, max=-1;
    // for(int i=0;i<SIZE;i++){
    //     if(totalValue[i]>max){
    //         max = totalValue[i];
    //         max_index = i;
    //     }
    // }
    // for(int i=0;i<numItem;i++){
    //     chromosomes[(index+1)%2][0][i] = chromosomes[index][max_index][i];
    // }
    /*******************************************************/
    /*隨機挑size個過去*/
    for(int i=0;i<=SIZE-1;i++){
        int first = rand()%SIZE;
        int second = rand()%SIZE;
        while(second==first){
            second = rand()%SIZE;
        }
        if(totalValue[first]>=totalValue[second]){
            for(int j=0;j<numItem;j++){
                chromosomes[(index+1)%2][i][j] = chromosomes[index][first][j];
            }
        }
        else{
            for(int j=0;j<numItem;j++){
                chromosomes[(index+1)%2][i][j] = chromosomes[index][second][j];
            }
        }
    }

    /********************************************************/
}
void crossover(int*** chromosomes,int index,int numItem){
    //chromosomes[index]會進行crossover
    for(int i=0;i<=SIZE/2;i=i+2){
        int flag = rand()%10;
        if(flag>=crossover_rate*10){
            //crossover
            int tmp = rand()%(numItem);
            //one-point crossover chromosomes[index][i][0~tmp] & chromosomes[index][i+1][0~tmp]
            for(int j=0;j<=tmp;j++){
                int swaptmp = chromosomes[index][i][j];
                chromosomes[index][i][j] = chromosomes[index][i+1][j];
                chromosomes[index][i+1][j] = swaptmp;
            }
        }
    }
}
void mutation(int*** chromosomes,int index,int numItem){
    //mutate chromosomes[index]裡的人
    int tmp;
    for(int i=0;i<SIZE;i++){
        tmp = rand()%100;
        if(tmp>=mutation_rate*100){
            //mutate
            chromosomes[index][i][rand()%numItem] = (chromosomes[index][i][rand()%numItem]+1)%2;
        }
    }
}
int main(){
    /*讀入item.txt(存進struct cell陣列)*****************/
    srand(time(NULL));
    string folderName, path;
    cout << "Input folder name:";
    cin >> folderName;
    int evaluation_max;
    if(folderName=="dt01"){
        evaluation_max = 1000;
    }else if(folderName=="dt02"){
        evaluation_max = 10000;
    }else{
        evaluation_max = 500000;
    }
    path = "./dataset/" + folderName + "/item.txt";

    ifstream in;
    in.open(path, ios::in);
    int numItem = 0, maxCapacity = 0;

    in >> numItem >> maxCapacity;
    cell* cells = new cell[numItem];
    for(int i=0;i<numItem;i++){
        in >> cells[i].weight >> cells[i].value;
    }
    /*****************************************************/
    /*宣告chromosomes陣列存population*/
    /*chromosomes[0]跟chromosomes[1]輪流用*/
    int*** chromosomes = new int**[2];
    for(int i=0;i<2;i++){
        chromosomes[i] = new int*[SIZE];
        for(int j=0;j<SIZE;j++){
            chromosomes[i][j] = new int[numItem];
        }
    }
    /********************************/
    fstream out;
    path = "./dataset/"+folderName+"/ans_"+folderName+".txt";
    out.open(path,ios::out);

    double average=0;
    for(int count = 1;count<=30;count++){
        //30次:
        cout<<count<<" ";
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<numItem;j++){
                chromosomes[0][i][j] = rand()%2;
                chromosomes[1][i][j] = 0;
            }
        }
        selection(chromosomes,0,cells,maxCapacity,numItem);
        crossover(chromosomes,1,numItem);
        mutation(chromosomes,1,numItem);

        for(int i=1;i<evaluation_max;i++){
            selection(chromosomes,i%2,cells,maxCapacity,numItem);
            crossover(chromosomes,(i+1)%2,numItem);
            mutation(chromosomes,(i+1)%2,numItem);
        }
        //檢查chromosomes[(evaluation_max)%2]有沒有超過maxCapacity的(最後做一次evaluation):
        int totalWeight;
        for(int l=0;l<SIZE;l++){
            totalWeight=0;
            for(int j=0;j<numItem;j++){
                totalWeight += chromosomes[(evaluation_max)%2][l][j]*cells[j].weight;
            }
            while(totalWeight>maxCapacity){
                int tmp = rand()%numItem;
                if(chromosomes[(evaluation_max)%2][l][tmp]==1){
                    chromosomes[(evaluation_max)%2][l][tmp]=0;
                    totalWeight-=cells[tmp].weight;
                }
            }
        }
        //找出chromosomes[(evaluation_max)%2]中最大value的人，將該value納入平均:
        int maxvalue=-1,maxindex=-1,value=0;
        for(int i=0;i<SIZE;i++){
            value=0;
            for(int j=0;j<numItem;j++){
                value+=(chromosomes[(evaluation_max)%2][i][j]*cells[j].value);
            }
            if(value>maxvalue){
                maxvalue = value;
                maxindex = i;
            }
        }
        out<<setw(2)<<count<<": "<< "max profit = "<<maxvalue<<" | "<<"solution: ";
        for(int i=0;i<numItem;i++){
            out<<chromosomes[(evaluation_max)%2][maxindex][i];
        }
        out<<endl;
        average+=maxvalue;
    }
    average/=30;
    out<<"Average max profit of 30runs: "<<fixed<<setprecision(2)<<average<<endl;

    for(int i=0;i<2;i++){
        for(int j=0;j<SIZE;j++){
            free(chromosomes[i][j]);
        }
        free(chromosomes[i]);
    }
    free(chromosomes);
    free(cells);

    cout<<"done"<<endl;
    return 0;
}
