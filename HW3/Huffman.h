#include <cstdio>
#include <cstring>
#include <cstdlib>


typedef struct _node{
    char symbol;
    int freq;
    struct _node *left;
    struct _node *right;
} NODE;

//NODE *u, *v, *w;

void Compress();
void Decompress();
