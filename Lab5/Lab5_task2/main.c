#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_CAPACITY 100
#define MAX_NUM 100
#define isDigit(x)  ( '0'<=(x) && (x)<='9' )

int readDigit(char **c) { //��ȡ�������ĺ����������ƶ��ַ�ָ�룩
    //ע�⣺������������ַ�ָ�������ƶ��������мǲ��ܴ����ַ�������׵�ַ�����򽫻ᶪʧ�������ݣ�Ӧ�ô����ַ�ָ��ĸ���!!!
    int ret = 0;
    do {
        ret *= 10; //��������һλ
        ret += **c - 48; //�Ѹ�λ������
        *c += sizeof(char); //�ַ�ָ��ǰ��һλ
        //����ָ����������⣿���*c++,�ӵ���sizeof(BT)������ʱ��BT�ǣ�����
    } while (isDigit(**c)); //��������һ�������֮ǰ
    return ret; //���صõ��Ĳ�����
}


typedef struct _list {
    int _elem[DEFAULT_CAPACITY]; //����
    int size; //��Ч��ģ
} List;

List *initList() { //��ʼ���б�
    List *ret = (List *) malloc(sizeof(List));
    int i;
    for (i = 0; i < DEFAULT_CAPACITY; i++) {
        ret->_elem[i] = 0;
    }
    ret->size = 0;
    return ret;
}

void clrList(List *ls) { //����б�
    ls->size = 0;
}

void insert(List *ls, int val) { //�б�β������
    ls->_elem[ls->size] = val;
    ls->size++;
}

List *merge(List *ls1, List *ls2) { //�鲢
    List *merged = initList();
    int p1 = 0, p2 = 0;
    //�ҵ���ͷ��С����һ�������ӵ��µ��б�����ԭ�б���ɾ����Ԫ��
    while (p1 < ls1->size && p2 < ls2->size) {
        if (ls1->_elem[p1] <= ls2->_elem[p2]) {
            insert(merged, ls1->_elem[p1]);
            p1++;
        } else {
            insert(merged, ls2->_elem[p2]);
            p2++;
        }
    }
    //ĳ���б��Ѿ�����β��
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
        if(strcmp(input,"-1")==0) break; //����-1�˳�
        //��������
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
    printf("�ϲ��������Ϊ:\n[");
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
