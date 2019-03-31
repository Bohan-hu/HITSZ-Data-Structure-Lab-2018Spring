//实验一：链式存储结构实现
//姓名：胡博涵 学号：SZ170110113 2017级计算机1班
//主要功能：添加、定位、删除、归并

//头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
//常量定义区
#define NUM_CLASSES 4 //班级数目

typedef struct _stu { //"学生信息"结构体定义
    char id[100]; //学号
    char name[100]; //姓名
    int score; //得分
} Student;

typedef struct _node { //节点类的定义
    Student *data; //数据域指针
    struct _node *next; //后继节点指针
} Node;

typedef struct _list { //链表类的定义
    Node *head;  //头节点
    Node *tail; //尾节点
} List;

Student *initStudent(char id[], char name[], int score) {//"学生"类的构造函数
    Student *ret = (Student *) malloc(sizeof(Student)); //申请空间
    assert(ret!=NULL);//申请失败直接退出
    //由传入参数进行初始化
    strcpy(ret->name, name);
    strcpy(ret->id, id);
    ret->score = score;
    //返回构造的对象
    return ret;
}

Node *initNode(Student *stu) { //节点类的构造函数时
    Node *ret = (Node *) malloc(sizeof(Node));
    assert(ret!=NULL);
    ret->next = NULL;
    ret->data = stu;
    return ret;
}

List *initList() {
    List *ret = (List *) malloc(sizeof(List));
    //为排序方便，头节点、尾节点分别设置为最大、最小整数值
    ret->head = initNode(initStudent(" ", " ", 0x7fffffff));
    ret->tail = initNode(initStudent(" ", " ", 0x80000000));
    //链接头尾
    ret->head->next = ret->tail;
    //返回创建后的链表
    return ret;
}

Node *locate(List *ls, Student *stu) { //定位插入的正确位置，使得插入之后链表的单调性不被改变
    Node *ptr = ls->head;
    while (ptr->next->data->score > stu->score && ptr->next != ls->tail) { //遍历链表，直到查找到成绩低于当前传入节点的第一个位置
        ptr = ptr->next;
    }
    return ptr;
}

void insert(Node *pred, Node *ins) { //在前驱节点pred后插入节点ins
    ins->next = pred->next;
    pred->next = ins;
}

void insert_Ranked(List *ls, Student *stu) {
    Node *ins = initNode(stu); //将Student对象封装成一个节点
    insert(locate(ls, stu), ins); //调用locate函数确定位置后插入
}

Node *searchByKey(List *ls, char *(*key)(Student *), char val[]) { //顺序查找算法
    Node *curr;
    for (curr = ls->head->next; curr != ls->tail; curr = curr->next)
        if (strcmp((*key)(curr->data), val) == 0) return curr;
    return NULL; //未找到，返回空
}

char *name(Student *stu) { //用于查找函数调用函数指针
    return stu->name;
}

char *id(Student *stu) { //用于查找函数调用函数指针
    return stu->id;
}

void printRecord(Student *stu) {
    printf("%s\t%s\t%d\n", stu->id, stu->name, stu->score);
}

void printList(List *ls) {
    Node *curr;
    for (curr = ls->head->next; curr != ls->tail; curr = curr->next) {
        printRecord(curr->data);
    }
}

List *merge(List *ls1, List *ls2) { //
    Node *p1 = ls1->head->next, *p2 = ls2->head->next; //初始化两个下标值
    List *merged = initList(); //初始化一个空表
    while (p1 != ls1->tail && p2 != ls2->tail) { //两者均未到表尾
        if (p1->data->score >= p2->data->score) {
            //插入大的那个元素，并将其指针后移，另一个不动
            insert_Ranked(merged, p1->data);
            p1 = p1->next;
        } else {
            insert_Ranked(merged, p2->data);
            p2 = p2->next;
        }
    }
    //将剩余的元素接到末尾
    while (p1 != ls1->tail) {
        insert_Ranked(merged, p1->data);
        p1 = p1->next;
    }
    while (p2 != ls2->tail) {
        insert_Ranked(merged, p2->data);
        p2 = p2->next;
    }
    return merged; //返回新表
}

int main() {
    //Test
    /*List *c1 = initList();
    Student *stu1 = initStudent("123", "David", 99);
    Student *stu2 = initStudent("145", "Tim", 220);
    Student *stu3 = initStudent("222", "Jack", 45);
    Student *stu4 = initStudent("jkd", "JKJ", 500);
    insert_Ranked(c1, stu1);
    insert_Ranked(c1, stu2);
    insert_Ranked(c1, stu3);
    insert_Ranked(c1, stu4);
    printList(c1);
    printf("\n\n");
    List *c2 = initList();
    Student *sstu1 = initStudent("123", "ASD", 9);
    Student *sstu2 = initStudent("145", "DFG", 2);
    Student *sstu3 = initStudent("222", "wEW", 0);
    Student *sstu4 = initStudent("jkd", "JddddJ", 1);
    insert_Ranked(c2, sstu1);
    insert_Ranked(c2, sstu2);
    insert_Ranked(c2, sstu3);
    insert_Ranked(c2, sstu4);
    printList(c2);
    printf("\n\n");
    List *c3 = merge(c1, c2);
    printList(c3);
    printf("\n\n");
    printRecord(searchByKey(c1, name, "Tim")->data);
    printf("Hello, World!\n");*/
    int i;
    //初始化四个班级的空表
    List *classes[NUM_CLASSES];
    for (i = 0; i < NUM_CLASSES; i++) {
        classes[i] = initList();
    }
    printf("Number:SZ170110113\nlab1: Linked List Implementation\n\n");
    do {
        int choice;
        printf("\n1.Input record\n"
                       "2.Search by ID\n"
                       "3.Search by name\n"
                       "4.List record of each class\n"
                       "5.List record of all classes\n"
                       "0.Exit\n"
                       "Please enter your choice:\n>>>");
        while (1) { //防止手贱
            if (scanf("%d", &choice) && 0 <= choice && 5 >= choice)
                break;
            fflush(stdin);
            printf("Error, Input again!\n");
        }
        switch (choice) {
            case 0: {
                return 0;
            }
            case 1: {
                int classnum;
                printf("Input the class:\n");
                while (1) {//防止输入异常
                    if (scanf("%d", &classnum) && 1 <= classnum && NUM_CLASSES >= classnum)
                        break;
                    fflush(stdin);
                    printf("Error, Input again!\n");
                }
                char name[100], number[100];
                printf("Input the number:\n");
                scanf("%s", number);
                printf("Input the name:\n");
                scanf("%s", name);
                int score;
                printf("Input the score:\n");
                while (1) { //防止输入异常
                    if (scanf("%d", &score))
                        break;
                    fflush(stdin);
                    printf("Error, Input again!\n");
                }
                Student *stu = initStudent(number, name, score);
                insert_Ranked(classes[classnum - 1], stu);
                printList(classes[classnum - 1]);
                break;
            }
            case 2: {
                int i, flag = 1; //设置标识flag，在找到后设置为0
                char key[100];
                printf("Input the ID:\n");
                scanf("%s", key);
                for (i = 0; i < NUM_CLASSES; i++) {
                    Node *result = searchByKey(classes[i], id, key);
                    if (result) {
                        flag = 0; //找到后设置为0
                        printf("Class:%d\t", i + 1);
                        printRecord(result->data);
                    }
                }
                if (flag) printf("Not Found!\n"); //四个班级中均未找到
                break;
            }
            case 3: {
                int i, flag = 1;
                char key[100];
                printf("Input the name:\n");
                scanf("%s", key);
                for (i = 0; i < NUM_CLASSES; i++) {
                    Node *result = searchByKey(classes[i], name, key);
                    if (result) {
                        flag = 0;
                        printf("Class:%d\t", i + 1);
                        printRecord(result->data);
                    }
                }
                if (flag) printf("Not Found!\n");
                break;
            }
            case 4: {
                int i;
                printf("Input the class:\n");
                while (1) { //防止输入异常
                    if (scanf("%d", &i) && 1 <= i && NUM_CLASSES >= i)
                        break;
                    fflush(stdin);
                    printf("Error, Input again!\n");
                }
                printf("Class:%d\n", i);
                printList(classes[i - 1]);
                break;
            }
            case 5: {
                List *merged = initList();
                int i;
                for (i = 0; i < NUM_CLASSES; i++) { //1，2归并后，再与3归并，最后与4归并
                    merged = merge(merged, classes[i]);
                }
                printList(merged);
                break;
            }
        }
    } while (1);

}
