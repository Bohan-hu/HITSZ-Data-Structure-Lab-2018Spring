//ʵ��һ��˳��洢�ṹʵ��
//������������ ѧ�ţ�SZ170110113 2017�������1��
//��Ҫ���ܣ���ӡ���λ��ɾ�����鲢

//ͷ�ļ�
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
//����������
#define NUM_CLASSES 4 //�༶��Ŀ
#define DEFAULT_CAPACITY 3 //Ĭ��˳���

typedef int Position; //Ԫ����˳����е�λ��


typedef struct _stu { //"ѧ����Ϣ"�ṹ�嶨��
    char id[100]; //ѧ��
    char name[100]; //����
    int score; //�÷�
} Student;

typedef struct _list { //˳���ṹ�嶨��
    Student *_elem;  //������
    int capacity; //������
    int size; //��Ч����
} List;

List *initList() { //˳���Ĺ��캯��
    List *ret = (List *) malloc(sizeof(List)); //����ձ�Ŀռ�
    assert(ret!=NULL);//����ʧ��ֱ���˳�
    //��Ĭ�ϴ�С��ʼ���ձ�������
    ret->_elem = (Student *) malloc(sizeof(Student) * DEFAULT_CAPACITY);
    int i;
    for (i = 0; i < DEFAULT_CAPACITY; i++) {
        ret->_elem[i].score = 0;
    }
    //��ʼ���������
    ret->capacity = DEFAULT_CAPACITY;
    ret->size = 0;
    return ret;
}

Student *initStudent(char *id, char *name, int score) { //"ѧ��"��Ĺ��캯��
    Student *ret = (Student *) malloc(sizeof(Student)); //����ռ�
    assert(ret!=NULL);//����ʧ��ֱ���˳�
    //�ɴ���������г�ʼ��
    strcpy(ret->id, id);
    strcpy(ret->name, name);
    ret->score = score;
    //���ع���Ķ���
    return ret;
}

void expand(List *ls) { //��������ݺ���������ʱ����
    if (ls->capacity < ls->size + 1) { //�������Ƿ������м��
        //��Ҫ����
        Student *oldElem = ls->_elem; //����ԭ���������ַ
        ls->capacity *= 2; //����2��
        ls->_elem = (Student *) malloc(
                ls->capacity * sizeof(Student)); //T_T  Update From: ls->_elem = (Student*)malloc(ls->capacity)!!!!!
        assert(ls->_elem!=NULL);//�����·���ʧ�ܣ�ֱ���˳�
        int i;
        for (i = 0; i < ls->size; i++) { //����Ԫ��
            ls->_elem[i] = oldElem[i];
        }
        free(oldElem);
        return;
    } else { //����ռ�δ����ֱ�ӷ���
        return;
    }
}

void insert(List *ls, Student stu, Position pos) { //�����Ա���ĳ��λ�ò���Ԫ��
    expand(ls); //�ȼ���Ƿ���Ҫ����
    int i;
    for (i = ls->size; i > pos; i--) { //�ճ�����Ŀռ�
        ls->_elem[i] = ls->_elem[i - 1];
    }
    ls->_elem[pos] = stu; //����Ԫ��
    ls->size++; //���´�С
}

Position locate(List *ls, Student stu) { //��λԪ�ز������ȷλ�ã�ʹ�����Ա�������
    int i = 0;
    while (i < ls->size && ls->_elem[i].score > stu.score) { //���ҳɼ����ڵ�ǰ����ڵ�ĵ�һ��λ��
        i++;
    }
    return i;
}

void insert_Ranked(List *ls, Student stu) { //�ڱ��������ǰ���²���
    insert(ls, stu, locate(ls, stu)); //����֮ǰд�õ�insert��locate�������в���
}

void printRecord(List *ls, Position i) { //���ĳѧ������Ϣ
    printf("%s\t%s\t%d\n", ls->_elem[i].id, ls->_elem[i].name, ls->_elem[i].score);
}

void printList(List *ls) { //���ĳ�༶����Ϣ
    int i;
    for (i = 0; i < ls->size; i++) {
        printRecord(ls, i); //����֮ǰд�õ�printRecord����
    }
}

Position searchBykey(List *ls, char *(*key)(Student *), char val[]) { //˳������㷨
    int i;
    for (i = 0; i < ls->size; i++) {
        if (strcmp((*key)(&(ls->_elem[i])), val) == 0) return i;
    }
    return -1; //δ�ҵ�������-1
}

char *name(Student *stu) { //���ڲ��Һ������ú���ָ��
    return stu->name;
}

char *id(Student *stu) { //���ڲ��Һ������ú���ָ��
    return stu->id;
}


List *merge(List *ls1, List *ls2) { //��·�鲢�㷨
    int p1 = 0, p2 = 0; //��ʼ�������±�ֵ
    List *merged = initList(); //��ʼ��һ���ձ�
    while (p1 < ls1->size && p2 < ls2->size) { //���߾�δ����β
        //�������Ǹ�Ԫ�أ�������ָ����ƣ���һ������
        if (ls1->_elem[p1].score >= ls2->_elem[p2].score) {
            insert_Ranked(merged, ls1->_elem[p1]);
            p1++;
        } else {
            insert_Ranked(merged, ls2->_elem[p2]);
            p2++;
        }
    }
    //��ʣ���Ԫ�ؽӵ�ĩβ
    while (p1 < ls1->size) {
        insert_Ranked(merged, ls1->_elem[p1]);
        p1++;
    }
    while (p2 < ls2->size) {
        insert_Ranked(merged, ls2->_elem[p2]);
        p2++;
    }
    return merged; //�����±�
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
    //��ʼ���ĸ��༶�Ŀձ�
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
        while (1) { //��ֹ�����쳣
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
                while (1) { //��ֹ�����쳣
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
                while (1) { //��ֹ�����쳣
                    if (scanf("%d", &score))
                        break;
                    fflush(stdin);
                    printf("Error, Input again!\n");
                }
                Student *stu = initStudent(number, name, score); //����������ݷ�װ��һ���µ�ѧ������
                insert_Ranked(classes[classnum - 1], *stu); //���ö������
                break;
            }
            case 2: {
                int i, flag = 1; //���ñ�ʶflag�����ҵ�������Ϊ0
                char key[100];
                printf("Input the ID:\n");
                scanf("%s", key);
                for (i = 0; i < NUM_CLASSES; i++) {
                    int result = searchBykey(classes[i], id, key);
                    if (result != -1) {
                        flag = 0; //�ҵ�������Ϊ0
                        printf("Class:%d\t", i + 1);
                        printRecord(classes[i], result);
                    }
                }
                if (flag) printf("Not Found!\n"); //�ĸ��༶�о�δ�ҵ�
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
                while (1) { //��ֹ�����쳣
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
                for (i = 0; i < NUM_CLASSES; i++) { //1��2�鲢������3�鲢�������4�鲢
                    merged = merge(merged, classes[i]);
                }
                printList(merged);
                break;
            }

        }
    } while (1);
}
