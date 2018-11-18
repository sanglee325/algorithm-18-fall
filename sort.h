#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void DataInput();
void DataOutput(double elapsed);

/*Sorting Algorithm Index Order 1 2 3 4*/
void InsertionSort(int a, int b);
void HeapSort(int n);
void QuickSort(int left, int right);
void ImplementedQuickSort(int front, int rear);

void adjust(int root, int n);
int partition(int left, int right);

int *Data;
char *AlgoIndex;
char *DataFile;
int N;
