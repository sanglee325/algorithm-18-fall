#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <queue>
#define SWAP(a, b){int temp; temp = a; a = b; b = temp;}

using namespace std;

char *Option, *Filename;
typedef struct _node{
    int freq;
    char data;
    unsigned int b;
    bool leaf;
    struct _node* left;
    struct _node* right;
}NODE;

bool operator<(NODE a, NODE b){
    return a.freq < b.freq;
}
NODE *Data;
NODE *head;

priority_queue<NODE> heap;

void Compress();
void createHeap();
void encode();
void postOrder(NODE *node, NODE *parent, NODE *target);

void Decompress();
int type;
int idx;

int main(int argc, char *argv[]){
    int idx;

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
    int length = 0, idx;
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
            Data[0].leaf = true;
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
                    Data[type - 1].leaf = true;
                }
            }
        }
    }//symbol and its number saved
    
    for(i = 0; i < type; i++){
        heap.push(Data[i]);
    }

    createHeap();
    free(Data);
    encode();

    /*
    for(i = 0; i < type; i++)
      printf("Data[%d]: %c, freq: %d\n", i, Data[i].data, Data[i].freq);
      */
    //need to make a trreeeeeeeeeee
}

void createHeap(){
    int i, j;
    NODE *tmp1, *tmp2;
    NODE *newNode;

    while(1){
        newNode = NULL; tmp1 = NULL; tmp2 = NULL;
        tmp1 = (NODE*)malloc(sizeof(NODE));
        tmp2 = (NODE*)malloc(sizeof(NODE));

        tmp1->freq = heap.top().freq;
        tmp1->data = heap.top().data;
        tmp1->left = heap.top().left; tmp1->right = heap.top().right;
        tmp1->leaf = heap.top().leaf;
        tmp1->b = 1;
        heap.pop();

        tmp2->freq = heap.top().freq;
        tmp2->data = heap.top().data;
        tmp2->left = heap.top().left; tmp2->right = heap.top().right;
        tmp2->leaf = heap.top().leaf;
        tmp2->b = 1;
        heap.pop();

        newNode = (NODE*)malloc(sizeof(NODE));
        newNode->freq = tmp1->freq + tmp2->freq;
        newNode->b = 1;
        newNode->leaf = false;

        if(tmp1->freq >= tmp2->freq){
            newNode->left = tmp1;
            newNode->right = tmp2;
        }
        else{
            newNode->right = tmp1;
            newNode->left = tmp2;
        }

        if(heap.empty()) break;
        heap.push(*newNode);
    }

    head = newNode; 
}

void encode(){
    NODE *tmp, *parent, *target;
    NODE *root;
    
    root = (NODE*)malloc(sizeof(NODE));

    root->left = head;
    root->right = NULL;
    
    parent = root;
    tmp = head;
    Data = (NODE*)malloc(sizeof(NODE) * type);

    idx = 0;
    postOrder(tmp, parent, target);

    for(int i = 0; i < type; i++){
        printf("Data[%d]\ndata: %c, freq: %d\n", i, Data[i].data, Data[i].freq);
        printf("bit: ");
//        printf("%d\n", Data[i].b);
        for(int j = 32; j >= 0; --j){ 
          printf("%d", (Data[i].b >> j)&1);
        }
        printf("\n");
    }

}

void searchLeaf(NODE *root, NODE *parent){


}

void postOrder(NODE *node, NODE *parent, NODE *target){
    if(node->left != NULL){
        node->left->b = node->b;
        node->left->b <<= 1;
        parent = node;
        postOrder(node->left, node, target);
    }
    if(node->right != NULL){
        node->right->b = node->b;
        node->right->b <<= 1;
        node->right->b |= 1;
        parent = node;
        postOrder(node->right, node, target);
    }
    if(node->leaf == true){
        Data[idx].data = node->data;
        Data[idx].b = node->b;
        Data[idx].freq = node->freq;
        Data[idx].leaf = true;
        idx++;
    }
}

void Decompress(){
}

