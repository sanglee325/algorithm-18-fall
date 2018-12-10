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
void createDTree();
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

    createZipFile();/*
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
    }*/
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
 /*   for(int i = 0; i < type; i++){
        printf("Data[%d]\ndata: %c, freq: %d\n", i, Data[i].data, Data[i].freq);
        printf("bit: ");
        printf("%d\n", Data[i].b);
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
    int idx, end, len, prevLen, sum;
    int i, j, leftOver;
    double total = 0, count = 0;
    char c;
    unsigned int buf, temp, prevBuf;
    int buf_idx = 0;
    bool full = false;

    in = fopen(Filename, "r");
    strcat(Filename, ".zz");
    out = fopen(Filename, "wb");

    for(idx = 0; idx < type; idx++)
        total += Data[idx].len * Data[idx].freq;

    fwrite(&total, sizeof(double), 1, out);
    fwrite(&type, sizeof(int), 1, out);
    for(int i = 0; i < type; i++){
        fwrite(&Data[i].data, sizeof(char), 1, out);
        fwrite(&Data[i].len, sizeof(int), 1, out);
        fwrite(&Data[i].b, sizeof(unsigned int), 1, out);
    }

    leftOver = 0;
    prevBuf = 0;
    while(1){
        len += leftOver;
        buf = 0;
        buf |= prevBuf;
        while(1){
            end = fscanf(in, "%c", &c);
            if(end == EOF) break;
            for(i = 0; i < type; i++){
                if(Data[i].data == c){ 
                    len += Data[i].len;
                    count += Data[i].len;
                    temp = Data[i].b;
                    break;
                }
            }
            if(len > 32){
                leftOver = len - 32;
                temp >>= leftOver;
                prevBuf = Data[idx].b;
                prevBuf <<= 32 - leftOver;
            }
            if(len == 32){
                prevBuf = 0;
                leftOver = 0;
            }
            else    
                temp <<= 32 - len;
            buf |= temp;
            if(count == total) break;
            if(len >= 32){
                len = 0;
                break;
            }
        }
        fwrite(&buf, sizeof(unsigned int), 1, out);
        if(end == EOF) break;
        if(count == total) break;
    }
        /*
        for(idx = 0; idx < type; idx++){
            if(Data[idx].data == c){
                OFbuf = temp = Data[idx].b; 
                break;
            }
        }
        len[buf_idx] += Data[idx].len;
        count += Data[idx].len;
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
        else if(len[buf_idx] == 32){
            full = true;
        }
        else{
            temp <<= 32 - len[buf_idx];
            buf[buf_idx] |= temp;
        }
        if(full == true || count == total){
            for(int j = 31; j >= 0; --j){ 
                printf("%d", (buf[buf_idx] >> j)&1);
                if(j % 4 == 0) printf(" ");
            }
            printf("\n");
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
    }*/
            
    fclose(in);
    fclose(out);

}

void createDTree(){
    int i, idxShift;
    unsigned int x, buf;
    NODE *newN, *temp, *prev;
    
    newN = (NODE*)malloc(sizeof(NODE));
    newN->left = newN->right = NULL;
    head = newN;

    for(i = 0; i < type; i++){
        temp = head;
        idxShift = Data[i].len - 1;
        x = Data[i].b;

        while(idxShift >= 0){
            buf = Data[i].b;
            x = (buf >> idxShift) & (~(~0 << 1));
            if(x == 0){
                if(temp->left == NULL){
                    newN = (NODE*)malloc(sizeof(NODE));
                    newN->left = newN->right = NULL;
                    temp->left = newN;
                }
                temp = temp->left;
            }
            if(x == 1){
                if(temp->right == NULL){
                    newN = (NODE*)malloc(sizeof(NODE));
                    newN->left = newN->right = NULL;
                    temp->right = newN;
                }
                temp = temp->right;
            }
            idxShift--;
        }
        temp->data = Data[i].data;
        temp->len = Data[i].len;
        temp->b = Data[i].b;
    }
}

void Decompress(){
    FILE *cc, *dd;
    char c;
    int len;
    double total;
    double count = 0;
    unsigned int buf, temp;
    int buf_idx = 0, idxShift = 0, i, j;
    bool pFlag = false;
    NODE *root, *target;

    cc = fopen(Filename, "rb");
    strcat(Filename, ".yy");
    dd = fopen(Filename, "w");
    fread(&total, sizeof(double), 1, cc);
    fread(&type, sizeof(int), 1, cc);

    Data = (NODE*)malloc(sizeof(NODE) * type);

    for(i = 0; i < type; i++){
        fread(&c, sizeof(char), 1, cc);
        Data[i].data = c;
        fread(&len, sizeof(int), 1, cc);
        Data[i].len = len;
        fread(&temp, sizeof(unsigned int), 1, cc);
        Data[i].b = temp;
    }
    createDTree();

    target = head;
    while(1){
        fread(&temp, sizeof(unsigned int), 1, cc);
        buf_idx = 31;
        while(buf_idx >= 0){
            buf = temp;
            buf = (buf >> buf_idx) & (~(~0 << 1));
            if(buf == 0)
                target = target->left;
            if(buf == 1)
                target = target->right;
            if(target->left == NULL && target->right == NULL){
                fprintf(dd, "%c", target->data);
                target = head;
            }
            buf_idx--;
            count++;
            if(count == total) break;
        }
        if(count == total) break;
    }





    
    /*
    for(i = 0; i < 2; i++){
        buf[i] = 0;
        len[i] = 32;
    }
    while(1){
        if(len[buf_idx] == 0){
            fread(&temp, sizeof(unsigned int), 1, cc);
            buf[buf_idx] = temp;
        }
        for(j = 0; j < 32; j++){
            for(i = 0; i < type; i++){
                if(Data[i].b == buf[buf_idx] && Data[i].len == len[buf_idx]){
                    fprintf(dd, "%c", Data[i].data);
                    count += len[buf_idx];
                    if(count == total) break;
                    pFlag = true;
                    break;
                }
            }
            if(count == total) break;
            if(pFlag == true){
                buf[buf_idx] = temp;
                buf[buf_idx] <<= j;
                buf[buf_idx] >>= j;
                numShift = j;
                pFlag = false;
            }
            else if(pFlag == false && j != 31){
                buf[buf_idx] >>= 1;
                len[buf_idx] -= 1;
            }
        }
        if(count == total) break;
        if(pFlag == false && j == 31){


        }
    }*/
    free(Data);
}


