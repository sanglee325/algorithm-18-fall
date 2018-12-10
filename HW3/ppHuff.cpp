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
    int max, child, len;
    struct _node* left;
    struct _node* right;
}NODE;

bool operator<(NODE a, NODE b){
    if(a.freq == b.freq){
        return a.child < b.child;
    }
    else
        return a.freq > b.freq;
}//작은 값이 top으로 오도록
NODE *Data;
NODE *head;

priority_queue<NODE> heap;

void Compress();
void createHeap();
void encode();
void postOrder(NODE *node, NODE *parent, NODE *target);
void createZipFile();

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
            Data[0].freq = Data[0].max = 1;
            Data[0].leaf = true;
            Data[0].child = Data[0].len = 1;
            type++;
        }
        else{
            for(i = 0; i < type; i++){
                if(Data[i].data == tmp){
                    newS = false;
                    Data[i].freq++;
                    Data[i].max++;
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
                    Data[type - 1].freq = Data[type - 1].max = 1;
                    Data[type - 1].leaf = true;
                    Data[type - 1].child = Data[type - 1].len = 1;
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

    createZipFile();
    for(i = 0; i < type; i++){
      printf("Data[%d]: %c, freq: %d, len: %d\n", i, Data[i].data, Data[i].freq, Data[i].len);
        //printf("Data[%d]\ndata: %c, freq: %d\n", i, Data[i].data, Data[i].freq);
        //printf("bit: ");
       // printf("%d\n", Data[i].b);
        for(int j = 31; j >= 0; --j){ 
          printf("%d", (Data[i].b >> j)&1);
          if(j % 4 == 0) printf(" ");
        }
        printf("\n");
    }
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
        tmp1->max = heap.top().max;
        tmp1->child = heap.top().child;
        tmp1->b = 0;
        heap.pop();

        tmp2->freq = heap.top().freq;
        tmp2->data = heap.top().data;
        tmp2->left = heap.top().left; tmp2->right = heap.top().right;
        tmp2->leaf = heap.top().leaf;
        tmp2->max = heap.top().max;
        tmp2->child = heap.top().child;
        tmp2->b = 0;
        heap.pop();

        newNode = (NODE*)malloc(sizeof(NODE));
        newNode->freq = tmp1->freq + tmp2->freq;
        newNode->b = 0;
        newNode->max = tmp1->max < tmp2->max ? tmp2->max : tmp1->max; 
        newNode->child = tmp1->child + tmp2->child;
        newNode->leaf = false;
       
        tmp1->len = tmp2->len = newNode->len = 1;

        if(tmp1->freq < tmp2->freq){
            newNode->left = tmp1;
            newNode->right = tmp2;
        }
        else if(tmp1->freq > tmp2->freq){
            newNode->right = tmp1;
            newNode->left = tmp2;
        }

        else if(tmp1->freq == tmp2->freq){
            if(tmp1->max > tmp2->max){
                newNode->left = tmp1;
                newNode->right = tmp2;
            }
            else{ 
                newNode->left = tmp2;
                newNode->right = tmp1;
            }
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
/*    for(int i = 0; i < type; i++){
        //printf("Data[%d]\ndata: %c, freq: %d\n", i, Data[i].data, Data[i].freq);
        //printf("bit: ");
       // printf("%d\n", Data[i].b);
        for(int j = 31; j >= 0; --j){ 
          printf("%d", (Data[i].b >> j)&1);
          if(j % 4 == 0) printf(" ");
        }
        printf("\n");
    }*/
}

void postOrder(NODE *node, NODE *parent, NODE *target){
    if(node->left != NULL){
        node->left->len = node->len + 1;
        node->left->b = node->b;
        node->left->b <<= 1;
        parent = node;
        postOrder(node->left, node, target);
    }
    if(node->right != NULL){
        node->right->len = node->len + 1;
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
        Data[idx].len = --(node->len);
        Data[idx].b = node->b;
        idx++;
    }
}
void createZipFile(){
    FILE *in, *out;
    int idx, end, len[2], sum;
    char c;
    unsigned int buf[2], temp, OFbuf = 0;
    int buf_idx = 0;
    bool full = false;

    for(int i = 0; i < 2; i++){
        buf[i] = 0;
        len[i] = 0;
    }
    in = fopen(Filename, "r");
    strcat(Filename, ".cc");
    out = fopen(Filename, "wb");
    while(1){
        end = fscanf(in, "%c", &c);
        if(end == EOF) break;
        for(idx = 0; idx < type; idx++){
            if(Data[idx].data == c){
                OFbuf = temp = Data[idx].b; 
                break;
            }
        }
        len[buf_idx] += Data[idx].len;
        if(len[buf_idx] > 32){
            temp >>= len[buf_idx] - 32;
            buf[buf_idx] |= temp;
            if(buf_idx == 0){
                len[1] = len[0] - 32;
                OFbuf <<= 32 - len[1];
                buf[1] |= OFbuf;
            }
            if(buf_idx == 1){
                len[0] = len[1] - 32;
                OFbuf <<= 32 - len[0];
                buf[0] |= OFbuf;
            }
            full = true;
        }
        else{
            temp <<= 32 - len[buf_idx];
            buf[buf_idx] |= temp;
        }
        if(full == true){
            fwrite(&buf[buf_idx], sizeof(unsigned int), 1, out);
            buf[buf_idx] = 0;
            len[buf_idx] = 0;
            if(buf_idx == 1){
                buf_idx = 0;
            }
            if(buf_idx == 0){
                buf_idx = 1;
            }
            full = false;
        }
        OFbuf = temp = 0;
    }
            
    fclose(in);
    fclose(out);

}

void Decompress(){
}

