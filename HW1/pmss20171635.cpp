#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

int *Data;
int N, LeftIdx, RightIdx;
char AlgoIndex;
char *DataFile;

typedef struct{
    int sum, left, right;
}SumIdx;

//Time Complexity O(n^2), case 1
int mssN2(const int A[], int N){
    int ThisSum, MaxSum, idx1, idx2;

    MaxSum = 0;
    for(idx1 = 0; idx1 < N; idx1++){
        ThisSum = 0;
        for(idx2 = idx1; idx2 < N; idx2++){
            ThisSum += A[idx2];

            if(ThisSum > MaxSum){
                MaxSum = ThisSum;
                LeftIdx = idx1;
                RightIdx = idx2;
            }
        }
    }

    return MaxSum;
}

int max3(int n1, int n2, int n3){
    return n1 < n2 ? (n2 < n3 ? n3 : n2) : (n1 < n3 ? n3 : n1);
}
//use divide-and-conquer strategy
//Time Complexity O(nlogn), case 2
SumIdx mssNlogN(const int A[], int Left, int Right){
    SumIdx MaxLeft, MaxRight;
    SumIdx MaxLeftBorder, MaxRightBorder;
    SumIdx Result;
    int Center, idx, MaxSum, LeftBorderSum, RightBorderSum;

    if( Left == Right ){
        if(A[Left] > 0){
            Result.sum = A[Left];
            Result.left = Result.right = Left;
            return Result;
        }
        else{
            Result.sum = 0;
            Result.left = Result.right = Left;
            return Result;
        }
    }

    Center = (Left + Right) / 2;
    MaxLeft = mssNlogN(A, Left, Center);
    MaxRight = mssNlogN(A, Center + 1, Right);

    MaxLeftBorder.sum = 0; LeftBorderSum = 0;
    MaxLeftBorder.right = MaxLeftBorder.left = Center;
    
    for(idx = Center; idx >= Left; idx--){
        LeftBorderSum += A[idx];

        if(LeftBorderSum > MaxLeftBorder.sum){
            MaxLeftBorder.sum = LeftBorderSum;
            MaxLeftBorder.left = idx;
        }
    }

    MaxRightBorder.sum = 0; RightBorderSum = 0;
    MaxRightBorder.left = MaxRightBorder.right = Center + 1;
    
    for(idx = Center + 1; idx <= Right; idx++){
        RightBorderSum += A[idx];

        if(RightBorderSum > MaxRightBorder.sum){
            MaxRightBorder.sum = RightBorderSum;
            MaxRightBorder.right = idx;
        }
    }

    MaxSum = max3(MaxLeft.sum, MaxRight.sum, MaxLeftBorder.sum + MaxRightBorder.sum);
    
    if(MaxSum ==  MaxLeftBorder.sum + MaxRightBorder.sum){
        Result.sum = MaxSum;
        Result.left = MaxLeftBorder.left;
        Result.right = MaxRightBorder.right;
    }
    else if(MaxSum == MaxLeft.sum){
        Result = MaxLeft;
    }
    else if(MaxSum == MaxRight.sum){
        Result = MaxRight;
    }
    return Result;
}

//Use Dynamic Programming Strategy
//Time Complexity O(n), case 3
int mssN(const int A[], int N){
    int ThisSum, MaxSum, idx = 0;

    LeftIdx = idx;
    ThisSum = MaxSum = 0;
    for(idx = 0; idx < N; idx++){
        ThisSum += A[idx];

        if(ThisSum > MaxSum){
            RightIdx = idx;
            MaxSum = ThisSum;
        }
        else if(ThisSum < 0){
            LeftIdx = RightIdx = idx;
            ThisSum = 0;
        }

    }

    return MaxSum;
}

void DataInput(){
    FILE *input;
    int idx;

    input = fopen(DataFile, "r");

    fscanf(input, "%d", &N);
    Data = (int*)malloc(sizeof(int) * (N));

    for(idx = 0; idx < N; idx++){
        fscanf(input, "%d", &Data[idx]);
    }
    fclose(input);
}

void DataOutput(int sum){
    char result[25] = "result_";
    FILE *output;

    strcat(result, DataFile);
    output = fopen(result, "w");

    fprintf(output, "Input Filename\t\t\t: %s\n", DataFile);    //1st input file name
    fprintf(output, "Algorithm index\t\t\t: %c\n", AlgoIndex);  //2nd algorithm index
    fprintf(output, "Input Size\t\t\t: %d\n", N);               //3rd input size
    fprintf(output, "Index of the leftmost number\t: %d\n", LeftIdx);    //4th index of the leftmost number in the subsequence found
    fprintf(output, "Index of the rightmost number\t: %d\n", RightIdx);   //5th index of the rightmost number in the subsequence found
    fprintf(output, "Sum of the subsequence\t\t: %d\n", sum); //6th sum of the subsequence
    fprintf(output, "Running Time\t\t\t: \n");                  //7th running time in milliseconds

    fclose(output);
}
int main(int argc, char *argv[]){
    int idx, sum;
    SumIdx temp;
    DataFile = argv[1];
    AlgoIndex = argv[2][0];

    DataInput();

    sum = 0;
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

    DataOutput(sum);
    free(Data);

    return 0;
}
