#include <stdio.h>
#include <time.h>

int main(){
      int num, N = 10000;
        FILE *fp = fopen("input00012.txt", "w");


          fprintf(fp, "%d ", N);
            srand((unsigned)time(NULL));

              for(int i = 0; i < N; i++){
                        num = rand() % N - (N/2);
                              fprintf(fp, "%d ", num);
                                }

                fclose(fp);

                  return 0;
}
