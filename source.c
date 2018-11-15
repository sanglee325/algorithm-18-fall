#include "Algo2.h"
#define SWAP(a, b){int temp; temp = a; a = b; b = temp;}
void DataInput(){
    FILE *input;
    int idx;

    input = fopen(DataFile, "r");

    //to save the memory, use malloc
    fscanf(input, "%d", &N);
    Data = (int*)malloc(sizeof(int) * (N));

    for(idx = 0; idx < N; idx++){
        fscanf(input, "%d", &Data[idx]);
    }
    fclose(input);
}
void DataOutput(double elapsed){
    char result[25] = "result_";
    int i;
    FILE *output;

    strcat(result, DataFile);
    output = fopen(result, "w");

    fprintf(output, "%s\n", DataFile);    //1st input file name
    fprintf(output, "%c\n", AlgoIndex);   //2nd algorithm index
    fprintf(output, "%d\n", N);           //3rd input size
    fprintf(output, "%lf\n", elapsed);    //7th running time in milliseconds
    
    for(i = 0; i < N; i++){
        fprintf(output, "%d ", Data[i]);
    }
    //sorted elements

    fclose(output);
}

//time complexity of O(n^2)
void BubbleSort(){
    int i, j, temp;

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            if(Data[j] > Data[j + 1]){
                SWAP(Data[j], Data[j + 1]);
            }
        }
    }

}
void HeapSort(){

}
void QuickSort(){

}
void CombinationSort(){
}
