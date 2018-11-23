#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main(){
    int num, N = 8000;
    FILE *fp = fopen("file_8.txt", "w");


    fprintf(fp, "%d ", N);
    srand((unsigned)time(NULL));

    for(int i = 0; i < N; i++){
        num = rand() % N;
        fprintf(fp, "%d ", num);
    }

    fclose(fp);

    return 0;
}

