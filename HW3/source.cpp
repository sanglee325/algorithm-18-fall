#include "Huffman.h"

extern char *Option, *Filename;

typedef struct _info{
    int num;
    char symbol;
}INFO;

INFO *Data;

void Compress(){

    FILE *input;
    int type = 0, length = 0, idx;
    int i, j;
    char tmp;
    bool newS;
    INFO *check;

    input = fopen(Filename, "r");

    while(1){
        fscanf(input, "%c", &tmp);
        length++;
        if(tmp == EOF) break;

        if(Data == NULL){
            Data = (INFO*)malloc(sizeof(INFO));
            Data[0].symbol = tmp;
            Data[0].num = 1;
            type++;
        }
        else{
            for(i = 0; i < type; i++){
                if(Data[i].symbol == tmp){
                    newS = false;
                    Data[i].num++;
                    break;
                }
                else newS = true;
            }

            if(newS){
                type++;
                check = Data;
                Data = (INFO*)realloc(Data, sizeof(INFO)*type);
                if(Data == 0){
                    Data = check;
                    printf("REALLOC ERROR\n");
                } // memory error check
                else{
                    Data[type - 1].symbol = tmp;
                    Data[type - 1].num = 1;
                }
            }
        }
    }//symbol and its number saved

    //need to make a trreeeeeeeeeee
}

void Decompress(){
}

