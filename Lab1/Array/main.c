//实验一：顺序存储结构实现
//姓名：胡博涵 学号：SZ170110113 2017级计算机1班
//主要功能：添加、定位、删除、归并

//头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
//常量定义区
#define NUM_CLASSES 4 //班级数目
#define DEFAULT_CAPACITY 3 //默认顺序表长

typedef int Position; //元素在顺序表中的位置


typedef struct _stu { //"学生信息"结构体定义
    char id[100]; //学号
    char name[100]; //姓名
    int score; //得分
} Student;

typedef struct _list { //顺序表结构体定义
    Student *_elem;  //数据域
    int capacity; //总容量
    int size; //有效容量
} List;

List *initList() { //顺序表的构造函数
    List *ret = (List *) malloc(sizeof(List)); //申请空表的空间
    assert(ret!=NULL);//申请失败直接退出
    //以默认大小初始化空表数据域
    ret->_elem = (Student *) malloc(sizeof(Student) * DEFAULT_CAPACITY);
    int i;
    for (i = 0; i < DEFAULT_CAPACITY; i++) {
        ret->_elem[i].score = 0;
    }
    //初始化各项参数
    ret->capacity = DEFAULT_CAPACITY;
    ret->size = 0;
    return ret;
}

Student *initStudent(char *id, char *name, int score) { //"学生"类的构造函数
    Student *ret = (Student *) malloc(sizeof(Student)); //申请空间
    assert(ret!=NULL);//申请失败直接退出
    //由传入参数进行初始化
    strcpy(ret->id, id);
    strcpy(ret->name, name);
    ret->score = score;
    //返回构造的对象
    return ret;
}

void expand(List *ls) { //数组的扩容函数，插入时调用
    if (ls->capacity < ls->size + 1) { //对数组是否将满进行检测
        //将要充满
        Student *oldElem = ls->_elem; //备份原先数据域地址
        ls->capacity *= 2; //扩容2倍
        ls->_elem = (Student *) malloc(
                ls->capacity * sizeof(Student)); //T_T  Update From: ls->_elem = (Student*)malloc(ls->capacity)!!!!!
        assert(ls->_elem!=NULL);//若重新分配失败，直接退出
        int i;
        for (i = 0; i < ls->size; i++) { //拷贝元素
            ls->_elem[i] = oldElem[i];
        }
        free(oldElem);
        return;
    } else { //如果空间未满，直接返回
        return;
    }
}

void insert(List *ls, Student stu, Position pos) { //向线性表中某个位置插入元素
    expand(ls); //先检测是否需要扩容
    int i;
    for (i = ls->size; i > pos; i--) { //空出插入的空间
        ls->_elem[i] = ls->_elem[i - 1];
    }
    ls->_elem[pos] = stu; //插入元素
    ls->size++; //更新大小
}

Position locate(List *ls, Student stu) { //定位元素插入的正确位置，使得线性表保持有序
    int i = 0;
    while (i < ls->size && ls->_elem[i].score > stu.score) { //查找成绩低于当前传入节点的第一个位置
        i++;
    }
    return i;
}

void insert_Ranked(List *ls, Student stu) { //在保持有序的前提下插入
    insert(ls, stu, locate(ls, stu)); //调用之前写好的insert和locate函数进行插入
}

void printRecord(List *ls, Position i) { //输出某学生的信息
    printf("%s\t%s\t%d\n", ls->_elem[i].id, ls->_elem[i].name, ls->_elem[i].score);
}

void printList(List *ls) { //输出某班级的信息
    int i;
    for (i = 0; i < ls->size; i++) {
        printRecord(ls, i); //调用之前写好的printRecord函数
    }
}

Position searchBykey(List *ls, char *(*key)(Student *), char val[]) { //顺序查找算法
    int i;
    for (i = 0; i < ls->size; i++) {
        if (strcmp((*key)(&(ls->_elem[i])), val) == 0) return i;
    }
    return -1; //未找到，返回-1
}

char *name(Student *stu) { //用于查找函数调用函数指针
    return stu->name;
}

char *id(Student *stu) { //用于查找函数调用函数指针
    return stu->id;
}


List *merge(List *ls1, List *ls2) { //二路归并算法
    int p1 = 0, p2 = 0; //初始化两个下标值
    List *merged = initList(); //初始化一个空表
    while (p1 < ls1->size && p2 < ls2->size) { //两者均未到表尾
        //插入大的那个元素，并将其指针后移，另一个不动
        if (ls1->_elem[p1].score >= ls2->_elem[p2].score) {
            insert_Ranked(merged, ls1->_elem[p1]);
            p1++;
        } else {
            insert_Ranked(merged, ls2->_elem[p2]);
            p2++;
        }
    }
    //将剩余的元素接到末尾
    while (p1 < ls1->size) {
        insert_Ranked(merged, ls1->_elem[p1]);
        p1++;
    }
    while (p2 < ls2->size) {
        insert_Ranked(merged, ls2->_elem[p2]);
        p2++;
    }
    return merged; //返回新表
}

int main() {
    /*List *c1 = initList();
    Student *stu1 = init_Student("123", "David", 99);
    Student *stu2 = init_Student("145", "Tim", 220);
    Student *stu3 = init_Student("222", "Jack", 45);
    Student *stu4 = init_Student("jkd", "JKJ", 500);
    Student *stu5 = init_Student("HBH", "Hubh", 1000);
    insert_Ranked(c1, *stu1);
    insert_Ranked(c1, *stu2);
    insert_Ranked(c1, *stu3);
    insert_Ranked(c1, *stu4);
    insert_Ranked(c1, *stu5);
    printList(c1);
    List *c2 = initList();
    Student *sstu1 = init_Student("123", "ASD", 9);
    Student *sstu2 = init_Student("145", "DFG", 2);
    Student *sstu3 = init_Student("222", "wEW", 0);
    Student *sstu4 = initStudent("jkd", "JddddJ", 1);
    insert_Ranked(c2, *sstu1);
    insert_Ranked(c2, *sstu2);
    insert_Ranked(c2, *sstu3);
    insert_Ranked(c2, *sstu4);
    printList(c2);
    List *c3 = merge(c1, c2);
    printList(c3);
    return 0;*/
    int i;
    //初始化四个班级的空表
    List *classes[NUM_CLASSES];
    for (i = 0; i < NUM_CLASSES; i++) {
        classes[i] = initList();
    }
    printf("Number:SZ170110113\nlab1: Array List Implementation\n\n");
    do {
        int choice;
        printf("\n1.Input record\n"
                       "2.Search by ID\n"
                       "3.Search by name\n"
                       "4.List record of a class\n"
                       "5.List record of all classes\n"
                       "0.Exit\n"
                       "Please enter your choice:\n>>>");
        while (1) { //防止输入异常
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
                while (1) { //防止输入异常
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
                Student *stu = initStudent(number, name, score); //将输入的数据封装成一个新的学生对象
                insert_Ranked(classes[classnum - 1], *stu); //将该对象插入
                break;
            }
            case 2: {
                int i, flag = 1; //设置标识flag，在找到后设置为0
                char key[100];
                printf("Input the ID:\n");
                scanf("%s", key);
                for (i = 0; i < NUM_CLASSES; i++) {
                    int result = searchBykey(classes[i], id, key);
                    if (result != -1) {
                        flag = 0; //找到后设置为0
                        printf("Class:%d\t", i + 1);
                        printRecord(classes[i], result);
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
                    int result = searchBykey(classes[i], name, key);
                    if (result != -1) {
                        flag = 0;
                        printf("Class:%d\t", i + 1);
                        printRecord(classes[i], result);
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
