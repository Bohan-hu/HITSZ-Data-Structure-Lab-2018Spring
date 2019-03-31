//ʵ��һ����ʽ�洢�ṹʵ��
//������������ ѧ�ţ�SZ170110113 2017�������1��
//��Ҫ���ܣ���ӡ���λ��ɾ�����鲢

//ͷ�ļ�
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
//����������
#define NUM_CLASSES 4 //�༶��Ŀ

typedef struct _stu { //"ѧ����Ϣ"�ṹ�嶨��
    char id[100]; //ѧ��
    char name[100]; //����
    int score; //�÷�
} Student;

typedef struct _node { //�ڵ���Ķ���
    Student *data; //������ָ��
    struct _node *next; //��̽ڵ�ָ��
} Node;

typedef struct _list { //������Ķ���
    Node *head;  //ͷ�ڵ�
    Node *tail; //β�ڵ�
} List;

Student *initStudent(char id[], char name[], int score) {//"ѧ��"��Ĺ��캯��
    Student *ret = (Student *) malloc(sizeof(Student)); //����ռ�
    assert(ret!=NULL);//����ʧ��ֱ���˳�
    //�ɴ���������г�ʼ��
    strcpy(ret->name, name);
    strcpy(ret->id, id);
    ret->score = score;
    //���ع���Ķ���
    return ret;
}

Node *initNode(Student *stu) { //�ڵ���Ĺ��캯��ʱ
    Node *ret = (Node *) malloc(sizeof(Node));
    assert(ret!=NULL);
    ret->next = NULL;
    ret->data = stu;
    return ret;
}

List *initList() {
    List *ret = (List *) malloc(sizeof(List));
    //Ϊ���򷽱㣬ͷ�ڵ㡢β�ڵ�ֱ�����Ϊ�����С����ֵ
    ret->head = initNode(initStudent(" ", " ", 0x7fffffff));
    ret->tail = initNode(initStudent(" ", " ", 0x80000000));
    //����ͷβ
    ret->head->next = ret->tail;
    //���ش����������
    return ret;
}

Node *locate(List *ls, Student *stu) { //��λ�������ȷλ�ã�ʹ�ò���֮������ĵ����Բ����ı�
    Node *ptr = ls->head;
    while (ptr->next->data->score > stu->score && ptr->next != ls->tail) { //��������ֱ�����ҵ��ɼ����ڵ�ǰ����ڵ�ĵ�һ��λ��
        ptr = ptr->next;
    }
    return ptr;
}

void insert(Node *pred, Node *ins) { //��ǰ���ڵ�pred�����ڵ�ins
    ins->next = pred->next;
    pred->next = ins;
}

void insert_Ranked(List *ls, Student *stu) {
    Node *ins = initNode(stu); //��Student�����װ��һ���ڵ�
    insert(locate(ls, stu), ins); //����locate����ȷ��λ�ú����
}

Node *searchByKey(List *ls, char *(*key)(Student *), char val[]) { //˳������㷨
    Node *curr;
    for (curr = ls->head->next; curr != ls->tail; curr = curr->next)
        if (strcmp((*key)(curr->data), val) == 0) return curr;
    return NULL; //δ�ҵ������ؿ�
}

char *name(Student *stu) { //���ڲ��Һ������ú���ָ��
    return stu->name;
}

char *id(Student *stu) { //���ڲ��Һ������ú���ָ��
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
    Node *p1 = ls1->head->next, *p2 = ls2->head->next; //��ʼ�������±�ֵ
    List *merged = initList(); //��ʼ��һ���ձ�
    while (p1 != ls1->tail && p2 != ls2->tail) { //���߾�δ����β
        if (p1->data->score >= p2->data->score) {
            //�������Ǹ�Ԫ�أ�������ָ����ƣ���һ������
            insert_Ranked(merged, p1->data);
            p1 = p1->next;
        } else {
            insert_Ranked(merged, p2->data);
            p2 = p2->next;
        }
    }
    //��ʣ���Ԫ�ؽӵ�ĩβ
    while (p1 != ls1->tail) {
        insert_Ranked(merged, p1->data);
        p1 = p1->next;
    }
    while (p2 != ls2->tail) {
        insert_Ranked(merged, p2->data);
        p2 = p2->next;
    }
    return merged; //�����±�
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
    //��ʼ���ĸ��༶�Ŀձ�
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
        while (1) { //��ֹ�ּ�
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
                while (1) {//��ֹ�����쳣
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
                Student *stu = initStudent(number, name, score);
                insert_Ranked(classes[classnum - 1], stu);
                printList(classes[classnum - 1]);
                break;
            }
            case 2: {
                int i, flag = 1; //���ñ�ʶflag�����ҵ�������Ϊ0
                char key[100];
                printf("Input the ID:\n");
                scanf("%s", key);
                for (i = 0; i < NUM_CLASSES; i++) {
                    Node *result = searchByKey(classes[i], id, key);
                    if (result) {
                        flag = 0; //�ҵ�������Ϊ0
                        printf("Class:%d\t", i + 1);
                        printRecord(result->data);
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
