#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define HEIGHT(x) (((x)==NULL)?0:(x)->height)
#define MAX(a, b) ((a)>(b)?(a):(b))
#define BalFac(x) ( HEIGHT((x)->lChild) - HEIGHT((x)->rChild)) //平衡因子
#define Balanced(x) ( (-2 < BalFac(x) )  &&  (BalFac(x) < 2) ) //平衡标志
#define LINE_MAX 300
#define MAX_NUM 1700000
#define MAX_STOCK 2500
int numbers = 0;
//股票代码_交易日期_开盘价_最高价_最低价_收盘价_交易量_其他若干信息
typedef struct _item {
    char raw[LINE_MAX]; //原始信息
    char id[10]; //股票名称
    long date; //交易日期
    double start_price; //开盘价
    double highest; //最高价
    double lowest; //最低价
    double end_price; //收盘价
    double quantity; //交易量
    char otherInfo[LINE_MAX]; //其他信息
} Item;

typedef long T;
typedef struct __node {
    T val; //存储的关键字
    Item *ptr; //指向条目地址
    int height; //高度
    struct __node *lChild;
    struct __node *rChild;
} Node;

typedef struct idx { //索引表中的元素 区间为[lo,hi)
    char id[LINE_MAX]; //股票的名称
    int lo; //对应有序区间的初始端
    int hi; //对应有序区间的末尾
    Node *root; //对应AVL树的树根
} Index;
int num_index = 0;

Index indexList[MAX_STOCK];
Item items[MAX_NUM];
Item *ptrs[MAX_NUM];

void showItem(Item *i) {
    printf("股票代码:%s\n", i->id);
    printf("交易日期:%ld\n", i->date);
    printf("开盘价:%g\n", i->start_price);
    printf("最高价:%g\n", i->highest);
    printf("最低价:%g\n", i->lowest);
    printf("收盘价:%g\n", i->end_price);
    printf("交易量:%.1f\n", i->quantity);
    printf("其他信息:%s", i->otherInfo);
    printf("\n");
}

void writeItem(char *filename, Item *i[]) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("创建文件失败.\n");
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
    //打开文件
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("打开文件失败!\n");
        exit(-1);
    } else {
        printf("打开文件成功!\n");
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

int partition(int lo, int hi, int pivotIndex) { //[lo,hi)开区间,长度为hi-lo
    double pivotValue = ptrs[pivotIndex]->quantity;
    //把割点放到末尾
    Item *tmp = ptrs[hi - 1];
    ptrs[hi - 1] = ptrs[pivotIndex];
    ptrs[pivotIndex] = tmp;
    //需要记录新的位置
    pivotIndex = lo;
    int i;
    for (i = lo; i < hi - 1; i++) {
        if (ptrs[i]->quantity > pivotValue) { //把大的都移动到左边
            tmp = ptrs[i];
            ptrs[i] = ptrs[pivotIndex];
            ptrs[pivotIndex] = tmp;
            pivotIndex++;
        }
    }
    //把割点元素归位
    tmp = ptrs[hi - 1];
    ptrs[hi - 1] = ptrs[pivotIndex];
    ptrs[pivotIndex] = tmp;
    return pivotIndex; //返回割点位置
}

int selectPivot(int lo, int hi) { //寻找关键点
    int mi = (hi - lo) / 2 + lo;
    int ret = lo;
    if (ptrs[ret]->quantity < ptrs[mi]->quantity) ret = mi;
    if (ptrs[ret]->quantity < ptrs[hi - 1]->quantity) ret = hi - 1;
    return ret;
}

void QuickSort(int lo, int hi) {
    if (lo == hi) return; //基线条件
    int pivotIndex = selectPivot(lo, hi); //主元位置（担心越界）
    int newIndex = partition(lo, hi, pivotIndex);
    QuickSort(lo, newIndex);
    QuickSort(newIndex + 1, hi);
}

void ShellSort(int lo, int hi) { //希尔排序
    int step, i, j;
    int len = hi - lo;
    Item *temp;
    for (step = len; step > 0; step = step >> 2)
        for (i = step; i < len; i++) {
            temp = ptrs[i];
            for (j = i - step; j >= 0 && ptrs[j]->quantity < temp->quantity; j -= step)
                ptrs[j + step] = ptrs[j]; // 找到插入位置并插入
            ptrs[j + step] = temp;
        }
}

int linearSearch(int lo, int hi, char *id, long date) { //线性查找
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

Node *initNode(Item *p, Node *l, Node *r) { //把传入的指针封装成二叉树结点
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
    //完成旋转操作
    Node *newroot = root->lChild;
    root->lChild = newroot->rChild;
    newroot->rChild = root;
    //更新高度
    newroot->height = MAX(HEIGHT(newroot->lChild), HEIGHT(newroot->rChild)) + 1;
    root->height = MAX(HEIGHT(root->lChild), HEIGHT(root->rChild)) + 1;
    return newroot; //替换原有的根节点
}

Node *rotate_RR(Node *root) { //与上面完全对称
    //完成旋转操作
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

Node *insert(Node *root, Item *p) { //AVL树递归插入算法
    long key = p->date;
    if (root == NULL) {
        root = initNode(p, NULL, NULL);
        if (root == NULL) { //空间申请失败
            printf("空间申请失败！\n");
            return NULL;
        }
    } else if (key < root->val) { //在左侧插入的情况
        root->lChild = insert(root->lChild, p); //递归进行节点插入
        if (!Balanced(root)) { //失衡之后重新调节
            if (key < root->lChild->val) { //同方向
                root = rotate_LL(root);
            } else { //反方向
                root = rotate_LR(root);
            }
        }
    } else if (key > root->val) { //在右侧插入的情况
        root->rChild = insert(root->rChild, p);
        if (!Balanced(root)) { //失衡之后重新调节
            if (key > root->rChild->val) { //同方向RR旋转
                root = rotate_RR(root);
            } else { //反方向RL旋转
                root = rotate_RL(root);
            }
        }
    } else { //插入节点已经存在
        //printf("插入节点%ld已经存在！\n", key);
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
    if (root == NULL) return NULL; //没有找到
    if (key == root->val) return root; //找到
    if (key < root->val) return AVLSearch(root->lChild, key); //在左子树寻找
    if (key > root->val) return AVLSearch(root->rChild, key); //在右子树寻找
    return NULL;
}

void getIndex() { //构建索引
    int lo = 0, hi = 0;
    //记录区间的开始位置和结束
    while (hi < numbers) {
        char cur_id[LINE_MAX];
        strcpy(cur_id, items[lo].id);
        while (strcmp(cur_id, items[hi].id) == 0 && hi < numbers) hi++; //记录区间结束位置
        //在索引表中记录
        indexList[num_index].lo = lo;
        indexList[num_index].hi = hi;
        strcpy(indexList[num_index].id, items[lo].id);
        //区间终点作为新的起点
        lo = hi;
        num_index++;
    }
}

int main(void) {
    int i;
    //读取文件
    clock_t start, finish;
    double duration;
    /***************读取文件******************/
    start = clock(); //计时开始
    openFile("data.txt");
    finish = clock(); //计时结束
    duration = (double) (finish - start) / CLOCKS_PER_SEC; //计算操作用时
    getIndex(); //构建索引
    printf("读取文件时间 %g s.\n", duration);
    int choice;
    do { //显示菜单
        printf(
                "\n1.快速排序\n"
                        "2.选择排序\n"
                        "3.希尔排序\n"
                        "4.创建索引+平衡二叉排序树(AVL)\n"
                        "5.线性查找 \n"
                        "6.索引+平衡二叉排序树(AVL)查找 \n"
                        "输入操作：");
        while (scanf("%d", &choice) != 1) { //处理异常输入
            fflush(stdin);
            printf("非法输入！\n输入操作：");
        }
        switch (choice) {
            case 1: //快速排序
            {
                printf("重置数据...\n");
                initPtrs(items, ptrs);  //将之前排过序的所有指针重置位置
                printf("开始快速排序(O(NlogN))\n");
                start = clock(); //计时开始
                QuickSort(0, numbers);
                finish = clock(); //计时结束
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //计算操作用时
                printf("排序时间: %g s.\n", duration);
                writeItem("result.txt", ptrs);
                printf("文件已输出。\n");
                break;
            }

            case 2: //选择排序
                printf("重置数据...\n");
                initPtrs(items, ptrs);  //将之前排过序的所有指针重置位置
                printf("开始选择排序(O(NlogN))\n");
                start = clock(); //计时开始
                SelectionSort(0, numbers);
                finish = clock(); //计时结束
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //计算操作用时
                printf("排序时间: %g s.\n", duration);
                writeItem("result.txt", ptrs);
                printf("文件已输出。\n");
                break;

            case 3: //希尔排序
                printf("重置数据...\n");
                initPtrs(items, ptrs);  //将之前排过序的所有指针重置位置
                printf("开始希尔排序(O(NlogN))\n");
                start = clock(); //计时开始
                ShellSort(0, numbers);
                finish = clock(); //计时结束
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //计算操作用时
                printf("排序时间: %g s.\n", duration);
                writeItem("result.txt", ptrs);
                printf("文件已输出。\n");
                break;

            case 4: //建立AVL
                printf("重置数据...\n");
                initPtrs(items, ptrs);  //将之前排过序的所有指针重置位置
                start = clock(); //计时开始
                printf("开始建立二叉平衡树索引...\n");
                for (i = 0; i < num_index; i++) { //对索引中的每一个股票进行操作
                    int j;
                    for (j = indexList[i].lo; j < indexList[i].hi; j++) //对股票所属区间的数据项进行建立
                        indexList[i].root = insert(indexList[i].root, ptrs[j]); //递归插入AVL树
                }
                finish = clock(); //计时结束
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //计算操作用时
                printf("建立二叉平衡树时间: %g s.\n", duration);
                break;

            case 5: //线性查找
            {
                printf("进行无序线性查找，重置数据...\n");
                initPtrs(items, ptrs);  //将之前排过序的所有指针重置位置，以测试线性查找
                char search_id[LINE_MAX];
                long date;
                printf("输入股票代码:");
                scanf("%s", search_id);
                printf("输入待查找的日期：");
                scanf("%ld", &date);
                start = clock(); //计时开始
                int i;
                i = linearSearch(0, numbers, search_id, date);
                finish = clock(); //计时结束
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //计算操作用时
                if(i!=-1){
                printf("查找成功!\n 查找时间: %g s.\n", duration);
                showItem(ptrs[i]);
                } else {
                    printf("查找失败!\n 查找时间: %g s.\n", duration);
                }
                break;
            }
            case 6: //二叉+索引搜索
            {
                char search_id[LINE_MAX];
                long date;
                printf("输入股票代码:");
                scanf("%s", search_id);
                printf("输入待查找的日期：");
                scanf("%ld", &date);

                /***************若没有进行过树的建立******************/
                if (indexList[0].root == NULL) {
                    printf("AVL树尚未创建，创建中...\n");
                    printf("重置数据...\n");
                    initPtrs(items, ptrs);  //将之前排过序的所有指针重置位置
                    start = clock(); //计时开始
                    printf("开始建立二叉平衡树索引...\n");
                    for (i = 0; i < num_index; i++) { //对索引中的每一个股票进行操作
                        int j;
                        for (j = indexList[i].lo; j < indexList[i].hi; j++) //对股票所属区间的数据项进行建立
                            indexList[i].root = insert(indexList[i].root, ptrs[j]); //递归插入AVL树
                    }
                    finish = clock(); //计时结束
                    duration = (double) (finish - start) / CLOCKS_PER_SEC; //计算操作用时
                    printf("建立二叉平衡树时间: %g s.\n", duration);
                }
                /************************************************/

                /***************查找部分******************/
                Node *result = NULL;
                start = clock(); //计时开始
                for (i = 0; i < num_index; i++) {
                    if (strcmp(indexList[i].id, search_id) == 0) {
                        result = AVLSearch(indexList[i].root, date);
                    }
                }
                finish = clock(); //计时结束
                duration = (double) (finish - start) / CLOCKS_PER_SEC; //计算操作用时
                /***************************************/


                if (result != NULL) { //查找成功
                    printf("查找成功!\n 查找时间: %g s.\n", duration);
                    showItem(result->ptr);  //输出结果
                } else { //查找失败
                    printf("查找失败!\n 查找时间: %g s.\n", duration);
                }
                break;
            }
            default:
                return 0;
        }
    } while (choice != 0);
}
