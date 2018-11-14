#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int *Data;
int N, LeftIdx, RightIdx;
char AlgoIndex;
char *DataFile;

//used for finding index in Algo Idx2
typedef struct{
    int sum, left, right;
}SumIdx;

//Time Complexity O(n^2), case 1
int mssN2(const int seq[], int N){
    int ThisSum, MaxSum, idx1, idx2;

    //initialize the MaxSum
    //calculate all of sums in sequence
    //if conditions exist for minus cases
    //If ThisSum > MaxSum renew the Max
    MaxSum = 0; 
    for(idx1 = 0; idx1 < N; idx1++){
        ThisSum = 0;
        for(idx2 = idx1; idx2 < N; idx2++){
            ThisSum += seq[idx2];

            if(idx1 == idx2 && ThisSum < 0){
                if(idx2 == 0 || ThisSum > MaxSum){
                    MaxSum = ThisSum;
                    LeftIdx = idx1;
                    RightIdx = idx2;
                }
            }
            if(ThisSum > MaxSum){
                MaxSum = ThisSum;
                LeftIdx = idx1;
                RightIdx = idx2;
            }
        }
    }

    return MaxSum;
}

//find the max number between 3 numbers
int max3(int n1, int n2, int n3){
    return n1 < n2 ? (n2 < n3 ? n3 : n2) : (n1 < n3 ? n3 : n1);
}

//use divide-and-conquer strategy
//Time Complexity O(nlogn), case 2
SumIdx mssNlogN(const int seq[], int Left, int Right){
    SumIdx MaxLeft, MaxRight;
    SumIdx MaxLeftBorder, MaxRightBorder;
    SumIdx Result;
    int Center, idx, MaxSum, LeftBorderSum, RightBorderSum;
    int PrevMaxSum;
    
    //return the seq[left] value when Left = Right
    if( Left == Right ){
            Result.sum = seq[Left];
            Result.left = Result.right = Left;
            return Result;
    }

    //Divide the Data Array into half, Find each MaxSum, Compare
    Center = (Left + Right) / 2;
    MaxLeft = mssNlogN(seq, Left, Center);
    MaxRight = mssNlogN(seq, Center + 1, Right);

    //To compare the variables need to be initialized
    //PrevMaxSum -> initialize to the value of starting point
    MaxLeftBorder.sum = 0; LeftBorderSum = 0;
    MaxLeftBorder.right = MaxLeftBorder.left = Center;
    PrevMaxSum = seq[Center];

    //move the index from center to the left side
    //LeftBorderSum == totalsum of leftside
    //If LeftBorderSum > MaxLeft.sum renew the Max struct
    for(idx = Center; idx >= Left; idx--){
        LeftBorderSum += seq[idx];

        if(LeftBorderSum >= MaxLeftBorder.sum){
            MaxLeftBorder.sum = LeftBorderSum;
            MaxLeftBorder.left = idx;
        }
        //exception for negative
        if(LeftBorderSum < 0 && PrevMaxSum < 0){
            if(idx == Center || LeftBorderSum >= MaxLeftBorder.sum)
              MaxLeftBorder.sum = LeftBorderSum;
        }
        PrevMaxSum = MaxLeftBorder.sum;
    }

    //Below works same as Above on the right side
    MaxRightBorder.sum = 0; RightBorderSum = 0;
    MaxRightBorder.left = MaxRightBorder.right = Center + 1;
    PrevMaxSum = seq[Center + 1];

    for(idx = Center + 1; idx <= Right; idx++){
        RightBorderSum += seq[idx];

        if(RightBorderSum > MaxRightBorder.sum){
            MaxRightBorder.sum = RightBorderSum;
            MaxRightBorder.right = idx;
        }
        if(RightBorderSum < 0 && PrevMaxSum < 0){
            if(idx == Center + 1 || RightBorderSum > MaxRightBorder.sum) 
                MaxRightBorder.sum = RightBorderSum;
        }
        PrevMaxSum = MaxRightBorder.sum;
    }

    MaxSum = max3(MaxLeft.sum, MaxRight.sum, MaxLeftBorder.sum + MaxRightBorder.sum);
   
    //If condition starts from Left (In case sums are same Algorithm requires the left index)
    if(MaxSum == MaxLeft.sum){
        Result = MaxLeft;
    }
    else if(MaxSum ==  MaxLeftBorder.sum + MaxRightBorder.sum){
        Result.sum = MaxSum;
        Result.left = MaxLeftBorder.left;
        Result.right = MaxRightBorder.right;
    }
    else if(MaxSum == MaxRight.sum){
        Result = MaxRight;
    }
    return Result;
}

//Use Dynamic Programming Strategy
//Time Complexity O(n), case 3
int mssN(const int seq[], int N){
    int ThisSum, MaxSum, PrevMSum, idx = 0;
    int TempLeftIdx = 0;

    //ThisSum is the value of Previous Sum
    //if ThisSum is negative initialize as 0
    LeftIdx = idx;
    ThisSum = MaxSum = 0;
    for(idx = 0; idx < N; idx++){
        ThisSum += seq[idx];

        if(ThisSum >= 0){
            if(ThisSum > MaxSum){
                LeftIdx = TempLeftIdx;
                RightIdx = idx;
                MaxSum = ThisSum;
            }
        }
        //exceptions for negative
        else if(ThisSum < 0){
            if(idx == 0 || ThisSum > MaxSum){
                MaxSum = ThisSum;
                LeftIdx = RightIdx = idx;
            }
            else if(MaxSum >= 0 && MaxSum != PrevMSum)
              LeftIdx = RightIdx = idx + 1;
            ThisSum = 0;
            TempLeftIdx = idx + 1;
        }
        //required to stop max index increasing (Previous max == MaxSum)
        PrevMSum = MaxSum;
    }

    return MaxSum;
}

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

void DataOutput(int sum, double elapsed){
    char result[25] = "result_";
    FILE *output;

    strcat(result, DataFile);
    output = fopen(result, "w");

    fprintf(output, "%s\n", DataFile);    //1st input file name
    fprintf(output, "%c\n", AlgoIndex);   //2nd algorithm index
    fprintf(output, "%d\n", N);           //3rd input size
    fprintf(output, "%d\n", LeftIdx);     //4th index of the leftmost number in the subsequence found
    fprintf(output, "%d\n", RightIdx);    //5th index of the rightmost number in the subsequence found
    fprintf(output, "%d\n", sum);         //6th sum of the subsequence
    fprintf(output, "%lf\n", elapsed);    //7th running time in milliseconds

    fclose(output);
}
int main(int argc, char *argv[]){
    int idx, sum;
    clock_t start, end;
    SumIdx temp;

    //DataFile = name of inputfile
    //Algoindex = Choosing algorithm to run
    DataFile = argv[1];
    AlgoIndex = argv[2][0];

    //Input data
    DataInput();

    sum = 0;

    //measure elapse time
    //case 1 - O(n^2), 2 - O(nlogn), 3 - (n)
    start = clock();
    switch (AlgoIndex) {
      case '1' : sum = mssN2(Data, N);
                 break;
      case '2' : temp = mssNlogN(Data, 0, N - 1);
                 LeftIdx = temp.left;
                 RightIdx = temp.right;
                 sum = temp.sum;
                 break;
      case '3' : sum = mssN(Data, N);
                 break;
    }
    end = clock();

    //create result file and free data
    DataOutput(sum, (double)(end-start)/CLOCKS_PER_SEC * 1000);
    free(Data);

    return 0;
}
