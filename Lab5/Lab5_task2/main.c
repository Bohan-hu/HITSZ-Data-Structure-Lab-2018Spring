#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_CAPACITY 100
#define MAX_NUM 100
#define isDigit(x)  ( '0'<=(x) && (x)<='9' )

int readDigit(char **c) { //读取操作数的函数（将会移动字符指针）
    //注意：这个函数对于字符指针做了移动，所以切记不能传入字符数组的首地址，否则将会丢失整个数据，应该传入字符指针的副本!!!
    int ret = 0;
    do {
        ret *= 10; //数字左移一位
        ret += **c - 48; //把个位加上来
        *c += sizeof(char); //字符指针前移一位
        //二级指针的运算问题？如果*c++,加的是sizeof(BT)，而此时的BT是？？？
    } while (isDigit(**c)); //在碰到下一个运算符之前
    return ret; //返回得到的操作数
}


typedef struct _list {
    int _elem[DEFAULT_CAPACITY]; //数据
    int size; //有效规模
} List;

List *initList() { //初始化列表
    List *ret = (List *) malloc(sizeof(List));
    int i;
    for (i = 0; i < DEFAULT_CAPACITY; i++) {
        ret->_elem[i] = 0;
    }
    ret->size = 0;
    return ret;
}

void clrList(List *ls) { //清除列表
    ls->size = 0;
}

void insert(List *ls, int val) { //列表尾部插入
    ls->_elem[ls->size] = val;
    ls->size++;
}

List *merge(List *ls1, List *ls2) { //归并
    List *merged = initList();
    int p1 = 0, p2 = 0;
    //找到表头中小的那一个，附加到新的列表，并从原列表中删除该元素
    while (p1 < ls1->size && p2 < ls2->size) {
        if (ls1->_elem[p1] <= ls2->_elem[p2]) {
            insert(merged, ls1->_elem[p1]);
            p1++;
        } else {
            insert(merged, ls2->_elem[p2]);
            p2++;
        }
    }
    //某个列表已经到达尾部
    while (p1 < ls1->size) {
        insert(merged, ls1->_elem[p1]);
        p1++;
    }
    while (p2 < ls2->size) {
        insert(merged, ls2->_elem[p2]);
        p2++;
    }
    return merged;
}

int main() {
    List *in = initList();
    List *total = initList();
    while (1) {
        char input[MAX_NUM];
        scanf("%s",input);
        if(strcmp(input,"-1")==0) break; //输入-1退出
        //处理输入
        char *curr = input;
        while (*curr != '\0') {
            if (!isDigit(*curr)) {
                curr++;
                continue;
            }
            insert(in, readDigit(&curr));
        }
        total = merge(in, total);
        clrList(in);
    }
    printf("合并后的数组为:\n[");
        for (int i = 0; i < total->size; i++) {
            printf("%d", total->_elem[i]);
            if (i < total->size - 1) {
                printf(",");
            }
        }
        printf("]");
        printf("\n");
    return 0;
}
