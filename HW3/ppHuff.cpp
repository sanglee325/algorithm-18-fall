#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <queue>

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
void copyNode(NODE *tmp, NODE heap);
//functions used to compress

void Decompress();
void createDTree();
//fuctions used to decompress

int type;
int idx;

int main(int argc, char *argv[]){
    //Filename = name of inputfile
    //Option = choose to compress or decompress
    Option = argv[1];
    Filename = argv[2];

    if(!strcmp(Option, "-c")) Compress();
    else if(!strcmp(Option, "-d")) Decompress();
    else printf("Option Error\n");  //error
    
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

    if(input == NULL){
        printf("INPUT FILE OPEN ERROR\n");
        return ;
    }//file open error
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
    }//symbol and its frequency saved
    
    for(i = 0; i < type; i++)
        heap.push(Data[i]);

    createHeap();
    free(Data);
    encode();
    createZipFile();
}

void copyNode(NODE *tmp, NODE heap){
    tmp->freq = heap.freq;
    tmp->data = heap.data;
    tmp->left = heap.left; tmp->right = heap.right;
    tmp->leaf = heap.leaf;
    tmp->max = heap.max;
    tmp->child = heap.child;
    tmp->b = 0;
}
void createHeap(){
    int i, j;
    NODE *tmp1, *tmp2;
    NODE *newNode;

    while(1){
        newNode = NULL; tmp1 = NULL; tmp2 = NULL;
        tmp1 = (NODE*)malloc(sizeof(NODE));
        tmp2 = (NODE*)malloc(sizeof(NODE));

        copyNode(tmp1, heap.top());
        heap.pop();
        copyNode(tmp2, heap.top());
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
        heap.push(*newNode); //push new node to priority queue
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
    int idx, end, len, prevLen;
    int i, leftOver;
    double total = 0, count = 0;
    char c;
    unsigned int buf, temp, prevBuf;
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
    //write data of code (header)

    len = 0;  leftOver = 0; prevBuf = 0; //initialize
    while(1){
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
                buf |= temp;
                prevBuf = Data[i].b;
                prevBuf <<= 32 - leftOver;
                len = leftOver;
                break;
            }
            if(len == 32){
                prevBuf = 0;
                leftOver = 0;
                len = 0;
                buf |= temp;
                break;
            }
            else{   
                temp <<= 32 - len;
                buf |= temp;
            }
            if(count == total) break;
        }
        fwrite(&buf, sizeof(unsigned int), 1, out);
        if(end == EOF) break;
        if(count == total) break;
    }//make compressed file
            
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
    }//create tree
}

void Decompress(){
    FILE *in, *out;
    char c;
    int len;
    double total, count = 0;
    unsigned int buf, temp;
    int buf_idx = 0, idxShift = 0, i, j;
    bool pFlag = false;
    NODE *root, *target;

    in = fopen(Filename, "rb");
    if(in == NULL){
        printf("INPUT FILE OPEN ERROR\n");
        return ;
    }
    strcat(Filename, ".yy");
    out = fopen(Filename, "w");
    fread(&total, sizeof(double), 1, in);
    fread(&type, sizeof(int), 1, in);

    Data = (NODE*)malloc(sizeof(NODE) * type);

    for(i = 0; i < type; i++){
        fread(&c, sizeof(char), 1, in);
        Data[i].data = c;
        fread(&len, sizeof(int), 1, in);
        Data[i].len = len;
        fread(&temp, sizeof(unsigned int), 1, in);
        Data[i].b = temp;
    }
    createDTree();//make huffman code tree

    target = head;
    while(1){
        fread(&temp, sizeof(unsigned int), 1, in);
        buf_idx = 31;
        while(buf_idx >= 0){
            buf = temp;
            buf = (buf >> buf_idx) & (~(~0 << 1));
            if(buf == 0)
                target = target->left;
            if(buf == 1)
                target = target->right;
            if(target->left == NULL && target->right == NULL){
                fprintf(out, "%c", target->data);
                target = head;
            }
            buf_idx--;
            count++;
            if(count == total) break;
        }
        if(count == total) break;
    }//decode the compressed file by bit

    fclose(in);
    fclose(out);
    free(Data);
}
