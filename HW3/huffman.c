#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char *Option, *Filename;
typedef struct _node{
    int freq;
    char data;
    struct _node* left;
    struct _node* right;
}NODE;

NODE *Data;
void Compress();
void Decompress();


int main(int argc, char *argv[]){
    int idx;
    clock_t start, end;

    //Filename = name of inputfile
    //Option = choose to compress or decompress
    Option = argv[1];
    Filename = argv[2];

    if(!strcmp(Option, "-c")) Compress();
    else if(!strcmp(Option, "-d")) Decompress();

    return 0;
}
void Compress(){
    FILE *input;
    int type = 0, length = 0, idx;
    int i, j, end;
    char tmp;
    bool newS;
    NODE *check;

    input = fopen(Filename, "r");

    while(1){
        end = fscanf(input, "%c", &tmp);
        length++;
        if(end == EOF) break;

        if(Data == NULL){
            Data = (NODE*)malloc(sizeof(NODE));
            Data[0].data = tmp;
            Data[0].freq = 1;
            type++;
        }
        else{
            for(i = 0; i < type; i++){
                if(Data[i].data == tmp){
                    newS = false;
                    Data[i].freq++;
                    break;
                }
                else newS = true;
            }

            if(newS){
                type++;
                check = Data;
                Data = (NODE*)realloc(Data, sizeof(NODE)*type);
                if(Data == 0){
                    Data = check;
                    printf("REALLOC ERROR\n");
                } // memory error check
                else{
                    Data[type - 1].data = tmp;
                    Data[type - 1].freq = 1;
                }
            }
        }
    }//symbol and its number saved
    
    for(i = 0; i < type; i++)
      printf("Data[%d]: %c, freq: %d\n", i, Data[i].data, Data[i].freq);
    //need to make a trreeeeeeeeeee
}
void createNode(){



}

void insertNode(){

}
void Decompress(){
}

