#include "sort.h"
#define SWAP(a, b){int temp; temp = a; a = b; b = temp;}

void DataInput(){
    FILE *input;
    int idx;

    input = fopen(DataFile, "r");

    //to save the memory, use malloc
    fscanf(input, "%d", &N);
    Data = (int*)malloc(sizeof(int) * (N + 1));

    for(idx = 1; idx < N + 1; idx++){
        fscanf(input, "%d", &Data[idx]);
    }
    fclose(input);
}
void DataOutput(double elapsed){
    char result[25] = "result_";
    int i;
    FILE *output;


    strcat(result, AlgoIndex);
    strcat(result,"_");
    strcat(result, DataFile);

    output = fopen(result, "w");

    fprintf(output, "%s\n", DataFile);    //1st input file name
    fprintf(output, "%s\n", AlgoIndex);   //2nd algorithm index
    fprintf(output, "%d\n", N);           //3rd input size
    fprintf(output, "%lf\n", elapsed);    //4th running time in seconds

    for(i = 1; i < N + 1; i++){
        fprintf(output, "%d ", Data[i]);
    }
    //sorted elements

    fclose(output);
}

//time complexity of O(n^2)
void InsertionSort(int a, int b){
    int i, j, temp;

    for(i = a; i <= b; i++){
        temp = Data[i];
        j = i;
        while((--j >= 0) && (temp < Data[j])){
            Data[j+1] = Data[j];
            Data[j] = temp;
        }
    }
}

void HeapSort(int n){
    int i;

    for(i = n/2; i > 0; i--)
        adjust(i, n);
    for(i = n - 1; i > 0; i--){
        SWAP(Data[1], Data[i + 1]);
        adjust(1, i);
    }

}

void adjust(int root, int n){
    int child, rootkey;
    rootkey = Data[root];
    child = 2 * root;

    while(child <= n){
        if((child < n) && (Data[child] < Data[child + 1]))
            child++;
        if(rootkey > Data[child]) break;
        else{
            Data[child / 2] = Data[child];
            child *= 2;
        }
    }
    Data[child / 2] = rootkey;
}
//Data starts from index 1, for easier heap sort
//makes heap structure using adjust

void QuickSort(int left, int right){
    int pivot;

    if(right - left > 0){
        pivot = partition(left, right);

        QuickSort(left, pivot - 1);
        QuickSort(pivot + 1, right);
    }
}

int partition(int left, int right){
    int i, pivot;
  
    pivot = left;

    for(i = left; i < right; i++){
        if(Data[i] < Data[right]){
            SWAP(Data[i], Data[pivot]);
            pivot++;
        }
    }

    SWAP(Data[right], Data[pivot]);
    return pivot;
}
//used in quick sort to choose pivot.
//the pivot is the most left index.
//returns the pivot value to main quick sort function

void MedianQuickSort(int front, int rear){
    int i, j, pivot;
    int mid = front + (rear - front) / 2;

    Sort3(front, mid, rear);

    if(rear - front + 1 > 3){
        pivot = Data[mid];
        SWAP(Data[mid], Data[rear - 1]);
        i = front;
        j = rear - 1;

        while(1){
            while(Data[++i] < pivot && i < rear);
            while(Data[--j] > pivot && front < j);
            if(i >= j) break;
            SWAP(Data[i], Data[j]);
        }
        SWAP(Data[i], Data[rear - 1]);
        MedianQuickSort(front, i - 1);
        MedianQuickSort(i + 1, rear);
    }

}

void Sort3(int front, int mid, int rear){
    if(Data[front] > Data[mid]) SWAP(Data[front], Data[mid]);
    if(Data[mid] > Data[rear]) SWAP(Data[mid], Data[rear]);
    if(Data[front] > Data[mid]) SWAP(Data[front], Data[mid]);
}
//in case of three-median the front, mid, rear data is sorted
//threeSort prevents the worst case of quick sort, pivor is mid index
//ex) non increasing
