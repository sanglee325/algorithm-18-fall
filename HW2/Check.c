#include <stdio.h>
#include <stdlib.h>

int *Data;
int main(int argc, char *argv[]){
    int idx, N;
    FILE *fp;
    char *DataFile;

    //DataFile = name of inputfile
    //Algoindex = Choosing algorithm to run
    DataFile = argv[1];

    fp = fopen(DataFile, "r");


    fscanf(fp, "%d", &N);
    Data = (int*)malloc(N * sizeof(int));
    
    for(int i = 0; i < N; i++)
      fscanf(fp, "%d", &Data[i]);

    for(int i = 0; i < N; i++){
        for(int j = i; j < N; j++)
          if(Data[i] > Data[j])
            printf("Data[%d] > Data[%d] FUXX!\n", i, j);
    }
  
    //create result file, free memory
    free(Data);

    return 0;
}
