#include "Huffman.h"

int main(int argc, char *argv[]){
    int idx;
    clock_t start, end;

    //Filename = name of inputfile
    //Option = choose to compress or decompress
    Option = argv[1];
    Filename = argv[2];

    if(!strcmp(Option, "-c")) Compress();
    else if(!strcmp(Option, "-d") Decompress();

    return 0;
}
