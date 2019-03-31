#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define HEIGHT(x) (((x)==NULL)?0:(x)->height)
#define MAX(a, b) ((a)>(b)?(a):(b))
#define BalFac(x) ( HEIGHT((x)->lChild) - HEIGHT((x)->rChild)) //ƽ������
#define Balanced(x) ( (-2 < BalFac(x) )  &&  (BalFac(x) < 2) ) //ƽ���־
#define LINE_MAX 300
#define MAX_NUM 1700000
#define MAX_STOCK 2500
int numbers = 0;
//��Ʊ����_��������_���̼�_��߼�_��ͼ�_���̼�_������_����������Ϣ
typedef struct _item {
    char raw[LINE_MAX]; //ԭʼ��Ϣ
    char id[10]; //��Ʊ����
    long date; //��������
    double start_price; //���̼�
    double highest; //��߼�
    double lowest; //��ͼ�
    double end_price; //���̼�
    double quantity; //������
    char otherInfo[LINE_MAX]; //������Ϣ
} Item;

typedef long T;
typedef struct __node {
    T val; //�洢�Ĺؼ���
    Item *ptr; //ָ����Ŀ��ַ
    int height; //�߶�
    struct __node *lChild;
    struct __node *rChild;
} Node;

typedef struct idx { //�������е�Ԫ�� ����Ϊ[lo,hi)
    char id[LINE_MAX]; //��Ʊ������
    int lo; //��Ӧ��������ĳ�ʼ��
    int hi; //��Ӧ���������ĩβ
    Node *root; //��ӦAVL��������
} Index;
int num_index = 0;

Index indexList[MAX_STOCK];
Item items[MAX_NUM];
Item *ptrs[MAX_NUM];

void showItem(Item *i) {
    printf("��Ʊ����:%s\n", i->id);
    printf("��������:%ld\n", i->date);
    printf("���̼�:%g\n", i->start_price);
    printf("��߼�:%g\n", i->highest);
    printf("��ͼ�:%g\n", i->lowest);
    printf("���̼�:%g\n", i->end_price);
    printf("������:%.1f\n", i->quantity);
    printf("������Ϣ:%s", i->otherInfo);
    printf("\n");
}

void writeItem(char *filename, Item *i[]) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("�����ļ�ʧ��.\n");
    }
    int cnt = 0;
    while (cnt < numbers) {
        fprintf(fp, "%s\n", i[cnt]->raw);
        cnt++;
    }
    fclose(fp);
}

void openFile(char *filename) {
    char line[LINE_MAX];
    char *curr;
    //���ļ�
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("���ļ�ʧ��!\n");
        exit(-1);
    } else {
        printf("���ļ��ɹ�!\n");
    }
    do {
        fscanf(fp, "%s", line);
        curr = line;
        memcpy(items[numbers].raw, line, LINE_MAX);
        curr = strtok(curr, "_");
        strcpy(items[numbers].id, curr);
        curr = strtok(NULL, "_");
        items[numbers].date = atol(curr);
        curr = strtok(NULL, "_");
        items[numbers].start_price = atof(curr);
        curr = strtok(NULL, "_");
        items[numbers].highest = atof(curr);
        curr = strtok(NULL, "_");
        items[numbers].lowest = atof(curr);
        curr = strtok(NULL, "_");
        items[numbers].end_price = atof(curr);
        curr = strtok(NULL, "_");
        items[numbers].quantity = atof(curr);
        curr = strtok(NULL, "");
        strcpy(items[numbers].otherInfo, curr);
        numbers++;
    } while (fgetc(fp) != EOF);
    fclose(fp);
}

void initPtrs(Item items[], Item *ptrs[]) {
    int i;
    for (i = 0; i < numbers; i++) {
        ptrs[i] = &items[i];
    }
}

void SelectionSort(int lo, int hi) {
    int maxPos = 0;
    Item *tmp;
    int i;
    for (i = 0; i < hi - 1; i++) {
        maxPos = i;
        int j;
        for (j = i + 1; j < hi; j++) { //Select the maximum
            if (ptrs[maxPos]->quantity < ptrs[j]->quantity) {
                maxPos = j;
            }
        }
        tmp = ptrs[i];
        ptrs[i] = ptrs[maxPos];
        ptrs[maxPos] = tmp;
    }
}

int partition(int lo, int hi, int pivotIndex) { //[lo,hi)������,����Ϊhi-lo
    double pivotValue = ptrs[pivotIndex]->quantity;
    //�Ѹ��ŵ�ĩβ
    Item *tmp = ptrs[hi - 1];
    ptrs[hi - 1] = ptrs[pivotIndex];
    ptrs[pivotIndex] = tmp;
    //��Ҫ��¼�µ�λ��
    pivotIndex = lo;
    int i;
    for (i = lo; i < hi - 1; i++) {
        if (ptrs[i]->quantity > pivotValue) { //�Ѵ�Ķ��ƶ������
            tmp = ptrs[i];
            ptrs[i] = ptrs[pivotIndex];
            ptrs[pivotIndex] = tmp;
            pivotIndex++;
        }
    }
    //�Ѹ��Ԫ�ع�λ
    tmp = ptrs[hi - 1];
    ptrs[hi - 1] = ptrs[pivotIndex];
    ptrs[pivotIndex] = tmp;
    return pivotIndex; //���ظ��λ��
}

int selectPivot(int lo, int hi) { //Ѱ�ҹؼ���
    int mi = (hi - lo) / 2 + lo;
    int ret = lo;
    if (ptrs[ret]->quantity < ptrs[mi]->quantity) ret = mi;
    if (ptrs[ret]->quantity < ptrs[hi - 1]->quantity) ret = hi - 1;
    return ret;
}

void QuickSort(int lo, int hi) {
    if (lo == hi) return; //��������
    int pivotIndex = selectPivot(lo, hi); //��Ԫλ�ã�����Խ�磩
    int newIndex = partition(lo, hi, pivotIndex);
    QuickSort(lo, newIndex);
    QuickSort(newIndex + 1, hi);
}

void ShellSort(int lo, int hi) { //ϣ������
    int step, i, j;
    int len = hi - lo;
    Item *temp;
    for (step = len; step > 0; step = step >> 2)
        for (i = step; i < len; i++) {
            temp = ptrs[i];
            for (j = i - step; j >= 0 && ptrs[j]->quantity < temp->quantity; j -= step)
                ptrs[j + step] = ptrs[j]; // �ҵ�����λ�ò�����
            ptrs[j + step] = temp;
        }
}

int linearSearch(int lo, int hi, char *id, long date) { //���Բ���
    int i;
    for (i = lo; i < hi; i++) {
        if (ptrs[i]->date == date && strcmp(id, ptrs[i]->id) == 0) {
            return i;
        }
    }
    return -1;
}

int depth(Node *root) {
    if (root == NULL) return 0;
    else return (MAX(depth(root->lChild), depth(root->rChild)) + 1);
}

Node *initNode(Item *p, Node *l, Node *r) { //�Ѵ����ָ���װ�ɶ��������
    Node *ret = (Node *) malloc(sizeof(Node));
    if (ret == NULL) {
        return NULL;
    }
    ret->ptr = p;
    ret->val = p->date;
    ret->height = 0;
    ret->lChild = l;
    ret->rChild = r;
    return ret;
}

Node *rotate_LL(Node *root) {
    //�����ת����
    Node *newroot = root->lChild;
    root->lChild = newroot->rChild;
    newroot->rChild = root;
    //���¸߶�
    newroot->height = MAX(HEIGHT(newroot->lChild), HEIGHT(newroot->rChild)) + 1;
    root->height = MAX(HEIGHT(root->lChild), HEIGHT(root->rChild)) + 1;
    return newroot; //�滻ԭ�еĸ��ڵ�
}

Node *rotate_RR(Node *root) { //��������ȫ�Գ�
    //�����ת����
    Node *newroot = root->rChild;
    root->rChild = newroot->lChild;
    newroot->lChild = root;
    newroot->height = MAX(HEIGHT(newroot->lChild), HEIGHT(newroot->rChild)) + 1;
    root->height = MAX(HEIGHT(root->lChild), HEIGHT(root->rChild)) + 1;
    return newroot;
}

Node *rotate_LR(Node *root) {
    root->lChild = rotate_RR(root->lChild);
    Node *newroot = rotate_LL(root);
    return newroot;
}

Node *rotate_RL(Node *root) {
    root->rChild = rotate_LL(root->rChild);
    Node *newroot = rotate_RR(root);
    return newroot;
}

Node *insert(Node *root, Item *p) { //AVL���ݹ�����㷨
    long key = p->date;
    if (root == NULL) {
        root = initNode(p, NULL, NULL);
        if (root == NULL) { //�ռ�����ʧ��
            printf("�ռ�����ʧ�ܣ�\n");
            return NULL;
        }
    } else if (key < root->val) { //������������
        root->lChild = insert(root->lChild, p); //�ݹ���нڵ����
        if (!Balanced(root)) { //ʧ��֮�����µ���
            if (key < root->lChild->val) { //ͬ����
                root = rotate_LL(root);
            } else { //������
                root = rotate_LR(root);
            }
        }
    } else if (key > root->val) { //���Ҳ��������
        root->rChild = insert(root->rChild, p);
        if (!Balanced(root)) { //ʧ��֮�����µ���
            if (key > root->rChild->val) { //ͬ����RR��ת
                root = rotate_RR(root);
            } else { //������RL��ת
                root = rotate_RL(root);
            }
        }
    } else { //����ڵ��Ѿ�����
        //printf("����ڵ�%ld�Ѿ����ڣ�\n", key);
    }
    root->height = MAX(HEIGHT(root->lChild), HEIGHT(root->rChild)) + 1;
    //printTree(root);
    return root;
}

void traverseIn(Node *root) {
    if (root == NULL) return;
    traverseIn(root->lChild);
    printf("%ld\n", root->val);
    traverseIn(root->rChild);
}

Node *AVLSearch(Node *root, T key) {
    if (root == NULL) return NULL; //û���ҵ�
    if (key == root->val) return root; //�ҵ�
    if (key < root->val) return AVLSearch(root->lChild, key); //��������Ѱ��
    if (key > root->val) return AVLSearch(root->rChild, key); //��������Ѱ��
    return NULL;
}

void getIndex() { //��������
    int lo = 0, hi = 0;
    //��¼����Ŀ�ʼλ�úͽ���
    while (hi < numbers) {
        char cur_id[LINE_MAX];
        strcpy(cur_id, items[lo].id);
        while (strcmp(cur_id, items[hi].id) == 0 && hi < numbers) hi++; //��¼�������λ��
        //���������м�¼
        indexList[num_index].lo = lo;
        indexList[num_index].hi = hi;
        strcpy(indexList[num_index].id, items[lo].id);
        //�����յ���Ϊ�µ����
        lo = hi;
        num_index++;
    }
}

int main(void) {
    int i;
    //��ȡ�ļ�
    clock_t start, finish;
    double duration;
    /***************��ȡ�ļ�******************/
    start = clock(); //��ʱ��ʼ
    openFile("data.txt");
    finish = clock(); //��ʱ����
    duration = (double) (finish - start) / CLOCKS_PER_SEC; //���������ʱ
    getIndex(); //��������
    printf("��ȡ�ļ�ʱ�� %g s.\n", duration);
    int choice;
    do { //��ʾ�˵�
        printf(
                "\n1.��������\n"
                        "2.ѡ������\n"
                        "3.ϣ������\n"
                        "4.��������+ƽ�����������(AVL)\n"
                        "5.���Բ��� \n"
                        "6.����+ƽ�����������(AVL)���� \n"
                        "���������");
        while (scanf("%d", &choice) != 1) { //�����쳣����
            fflush(stdin);
            printf("�Ƿ����룡\n���������");
        }
        switch (choice) {
            case 1: //��������
            {
                printf("��������...\n");
                initPtrs(items, ptrs);  //��֮ǰ�Ź��������ָ������λ��
                printf("��ʼ��������(O(NlogN))\n");
                start = clock(); //��ʱ��ʼ
                QuickSort(0, numbers);
                finish = clock(); //��ʱ����
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //���������ʱ
                printf("����ʱ��: %g s.\n", duration);
                writeItem("result.txt", ptrs);
                printf("�ļ��������\n");
                break;
            }

            case 2: //ѡ������
                printf("��������...\n");
                initPtrs(items, ptrs);  //��֮ǰ�Ź��������ָ������λ��
                printf("��ʼѡ������(O(NlogN))\n");
                start = clock(); //��ʱ��ʼ
                SelectionSort(0, numbers);
                finish = clock(); //��ʱ����
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //���������ʱ
                printf("����ʱ��: %g s.\n", duration);
                writeItem("result.txt", ptrs);
                printf("�ļ��������\n");
                break;

            case 3: //ϣ������
                printf("��������...\n");
                initPtrs(items, ptrs);  //��֮ǰ�Ź��������ָ������λ��
                printf("��ʼϣ������(O(NlogN))\n");
                start = clock(); //��ʱ��ʼ
                ShellSort(0, numbers);
                finish = clock(); //��ʱ����
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //���������ʱ
                printf("����ʱ��: %g s.\n", duration);
                writeItem("result.txt", ptrs);
                printf("�ļ��������\n");
                break;

            case 4: //����AVL
                printf("��������...\n");
                initPtrs(items, ptrs);  //��֮ǰ�Ź��������ָ������λ��
                start = clock(); //��ʱ��ʼ
                printf("��ʼ��������ƽ��������...\n");
                for (i = 0; i < num_index; i++) { //�������е�ÿһ����Ʊ���в���
                    int j;
                    for (j = indexList[i].lo; j < indexList[i].hi; j++) //�Թ�Ʊ�����������������н���
                        indexList[i].root = insert(indexList[i].root, ptrs[j]); //�ݹ����AVL��
                }
                finish = clock(); //��ʱ����
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //���������ʱ
                printf("��������ƽ����ʱ��: %g s.\n", duration);
                break;

            case 5: //���Բ���
            {
                printf("�����������Բ��ң���������...\n");
                initPtrs(items, ptrs);  //��֮ǰ�Ź��������ָ������λ�ã��Բ������Բ���
                char search_id[LINE_MAX];
                long date;
                printf("�����Ʊ����:");
                scanf("%s", search_id);
                printf("��������ҵ����ڣ�");
                scanf("%ld", &date);
                start = clock(); //��ʱ��ʼ
                int i;
                i = linearSearch(0, numbers, search_id, date);
                finish = clock(); //��ʱ����
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //���������ʱ
                if(i!=-1){
                printf("���ҳɹ�!\n ����ʱ��: %g s.\n", duration);
                showItem(ptrs[i]);
                } else {
                    printf("����ʧ��!\n ����ʱ��: %g s.\n", duration);
                }
                break;
            }
            case 6: //����+��������
            {
                char search_id[LINE_MAX];
                long date;
                printf("�����Ʊ����:");
                scanf("%s", search_id);
                printf("��������ҵ����ڣ�");
                scanf("%ld", &date);

                /***************��û�н��й����Ľ���******************/
                if (indexList[0].root == NULL) {
                    printf("AVL����δ������������...\n");
                    printf("��������...\n");
                    initPtrs(items, ptrs);  //��֮ǰ�Ź��������ָ������λ��
                    start = clock(); //��ʱ��ʼ
                    printf("��ʼ��������ƽ��������...\n");
                    for (i = 0; i < num_index; i++) { //�������е�ÿһ����Ʊ���в���
                        int j;
                        for (j = indexList[i].lo; j < indexList[i].hi; j++) //�Թ�Ʊ�����������������н���
                            indexList[i].root = insert(indexList[i].root, ptrs[j]); //�ݹ����AVL��
                    }
                    finish = clock(); //��ʱ����
                    duration = (double) (finish - start) / CLOCKS_PER_SEC; //���������ʱ
                    printf("��������ƽ����ʱ��: %g s.\n", duration);
                }
                /************************************************/

                /***************���Ҳ���******************/
                Node *result = NULL;
                start = clock(); //��ʱ��ʼ
                for (i = 0; i < num_index; i++) {
                    if (strcmp(indexList[i].id, search_id) == 0) {
                        result = AVLSearch(indexList[i].root, date);
                    }
                }
                finish = clock(); //��ʱ����
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //���������ʱ
                /***************************************/


                if (result != NULL) { //���ҳɹ�
                    printf("���ҳɹ�!\n ����ʱ��: %g s.\n", duration);
                    showItem(result->ptr);  //������
                } else { //����ʧ��
                    printf("����ʧ��!\n ����ʱ��: %g s.\n", duration);
                }
                break;
            }
            default:
                return 0;
        }
    } while (choice != 0);
}
