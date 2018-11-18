#include "sort.h"

int main(int argc, char *argv[]){
    int idx;
    clock_t start, end;

    //DataFile = name of inputfile
    //Algoindex = Choosing algorithm to run
    DataFile = argv[1];
    AlgoIndex = argv[2];

    DataInput();

    start = clock();
    switch (AlgoIndex[0]) {
      case '1' : InsertionSort(1, N);
                 break;
      case '2' : HeapSort(N);
                 break;
      case '3' : QuickSort(0, N);
                 break;
      case '4' : ImplementedQuickSort(1, N);
                 break;
    }
    end = clock();
  
    //create result file, free memory
    DataOutput((double)(end-start)/CLOCKS_PER_SEC);
    free(Data);

    return 0;
}
