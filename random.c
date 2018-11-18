#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main(){
    int num, N = 50;
    FILE *fp = fopen("file_1_50.txt", "w");


    fprintf(fp, "%d ", N);
    srand((unsigned)time(NULL));

    for(int i = 0; i < N; i++){
        num = rand() % N;
        fprintf(fp, "%d ", num);
    }

    fclose(fp);

    return 0;
}

