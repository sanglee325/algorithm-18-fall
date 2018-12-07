#include <cstdio>
#include <cstring>
#include <cstdlib>

char *Option;
char *Filename;

typedef struct _node{
    char symbol;
    int freq;
    struct _node *left;
    struct _node *right;
} NODE;

NODE *u, *v, *w;

void Compress();
void Decompress();
