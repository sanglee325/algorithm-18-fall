#include "Algo2.h"

int main(int argc, char *argv[]){
    int idx;
    clock_t start, end;

    //DataFile = name of inputfile
    //Algoindex = Choosing algorithm to run
    DataFile = argv[1];
    AlgoIndex = argv[2][0];

    DataInput();

    start = clock();
    switch (AlgoIndex) {
      case '1' : BubbleSort();
                 break;
      case '2' : HeapSort(N);
                 break;
      case '3' : QuickSort(0, N);
                 break;
      case '4' : CombinationSort();
                 break;
    }
    end = clock();
  
    //create result file, free memory
    DataOutput((double)(end-start)/CLOCKS_PER_SEC);
    free(Data);

    return 0;
}
