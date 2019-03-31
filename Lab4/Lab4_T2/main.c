#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_VerNUM 50
#define MAX_LENGTH 100

typedef struct exp {
    char dividend[MAX_LENGTH]; // ������
    char divisor[MAX_LENGTH]; //����
    double result; //��
} Expression;
//���ݽṹ��ƣ�AdjMatrix[a][b]��ֵ��ʾa/b��ֵ

//�趨Ϊȫ�ֱ��������㺯�����в�������
double AdjMatrix[MAX_VerNUM][MAX_VerNUM]; //�ڽӾ���
char vertex[MAX_VerNUM][MAX_LENGTH]; //������ĸ�Ķ�Ӧ��ϵ
int num_Eqs; //��֪����ʽ����
int num_Eqs2; //δ֪���������
int num_Verx; //�������


int locate_Vex(char *str) { //�ڱ���ж�λ����
    for (int i = num_Verx - 1; i >= 0; i--) {
        if (strcmp(vertex[i], str) == 0)
            return i;
    }
    return -1;//û�ҵ�������-1
}

Expression *str2exp(char *str) { //�������ʽ
    Expression *ret = (Expression *) malloc(sizeof(Expression));
    char *curr = str;
    curr = strtok(curr, "/");
    strcpy(ret->dividend, curr);
    curr = strtok(NULL, "=");
    strcpy(ret->divisor, curr);
    curr = strtok(NULL, "");
    ret->result = atof(curr);
    return ret;
}

void addEdge(Expression *exp) { //�����±�
    int pos_devidend, pos_devisor; //�������ͳ�����λ��
    double result = exp->result;
    //��λ�������ͳ����ھ����еı��
    pos_devidend = locate_Vex(exp->dividend);
    pos_devisor = locate_Vex(exp->divisor);
    //ԭ�Ȳ����ڣ������
    if (pos_devidend == -1) { //��Ӷ���
        strcpy(vertex[num_Verx], exp->dividend);
        pos_devidend = num_Verx;
        num_Verx++;
    }
    if (pos_devisor == -1) { //��Ӷ���
        strcpy(vertex[num_Verx], exp->divisor);
        pos_devisor = num_Verx;
        num_Verx++;
    }

    //˫��Ȩ�ز�ͬ
    AdjMatrix[pos_devidend][pos_devisor] = result;
    AdjMatrix[pos_devisor][pos_devidend] = 1.0 / result;

}

int nextNbr(int isVisited[MAX_VerNUM][MAX_VerNUM],int row) {//Ѱ�ҵ�ǰ�ڵ�ĵ�һ�����Է��ʵ��ھ�
    for (int i = num_Verx - 1; i >= 0; i--) {
        if (AdjMatrix[row][i] != 0 && !isVisited[row][i]) {  //û�����ʹ�����ͨ
            isVisited[row][i] = 1;
            return i;
        }
    }
    return -1;
}

double calc(int from, int to) { //�����ֽ⣨·��ת���㷨��
    int isVisited[MAX_VerNUM][MAX_VerNUM]; //��Ƕ����Ƿ����
    memset(isVisited,0, sizeof(isVisited));
    double ret = 1;
    int Stack[MAX_VerNUM]; //��ų�������Ĳ���
    int top = -1; //ջ�����
    Stack[++top] = from;  //�����ջ
    while (top > -1) { //ջ�ǿ�ʱ
        from = nextNbr(isVisited,Stack[top]); //��λջ������һ���ڽӽڵ�
        if (from == -1) { //�Ѿ����ʵ����еĽڵ㣨����ͬ��
            top--; //����
        } //���û���ҵ�����ʹ�õĶ��㣬����ջ��
        else {
            Stack[++top] = from; //���ҵ��Ķ�����ջ
            if (from == to) { //�����յ�
                for (int i = 0; i < top; i++) { //������
                    ret = ret * AdjMatrix[Stack[i]][Stack[i + 1]];
                }
                return ret;
            }
        }
    }
    return -1; //�޷�����������򷵻�-1
}

int main() {


    char input[MAX_LENGTH] = "a/b=0.4,b/d=2,c/a=4"; //��������
    char input_2[MAX_LENGTH] = "a/d,b/c"; //��Ҫ�����ʽ��
    char temp[MAX_VerNUM][MAX_LENGTH]; //��ŷָ��ı��ʽ
    char temp_2[MAX_VerNUM][MAX_LENGTH]; //��ŷָ����Ҫ�����ʽ��


    printf("��������֪�ı��ʽ:");
    scanf("%s",input);
    //printf("%s\n",input);
    printf("�����������ı��ʽ:");
    scanf("%s",input_2);
    //printf("%s\n",input_2);

    //��������ʽ�и�
    char *curr = input;
    curr = strtok(curr, ",");
    while (curr != NULL) {
        strcpy(temp[num_Eqs], curr);
        num_Eqs++; //�������֪ʽ�ӵĸ���
        curr = strtok(NULL, ",");
    }
    char *curr2 = input_2;
    curr2 = strtok(curr2, ",");
    while (curr2 != NULL) {
        strcpy(temp_2[num_Eqs2], curr2);
        num_Eqs2++; //�����δ֪ʽ�ӵĸ���
        curr2 = strtok(NULL, ",");
    }

    for (int i = 0; i < num_Eqs; i++) { //�����ʽ�ַ��������󣬼����ڽӾ�����
        //printf("������ı��ʽ%d:", i);
        Expression *exp = str2exp(temp[i]); //�����ʽ�ַ������������ṹ��
        //printf("��������%s,����:%s,��:%g\n", exp->dividend, exp->divisor, exp->result);
        addEdge(exp); //��������ı��ʽ��ӽ�ͼ
    }

    // ������ʽ����
    // printf("�������\n");
    /*for (int i = 0; i < num_Verx; i++) {
        for (int j = 0; j < num_Verx; j++) {
            printf("%.3f   ", AdjMatrix[i][j]);
        }
        printf("\n");
    }*/

    for (int i = 0; i < num_Eqs2; i++) {
        //printf("������ļ���ʽ%d:", i);
        curr2 = temp_2[i];
        curr2 = strtok(curr2, "/");
        //printf("%s/", curr2);
        int pos1 = locate_Vex(curr2);//�ҳ�������
        curr2 = strtok(NULL, "");
        int pos2 = locate_Vex(curr2);//�ҳ�����
        //printf("%s=", curr2);
        //printf("��������%s,����:%s,��:%g\n", exp->dividend, exp->divisor, calc(exp->dividend,exp->divisor));
        printf("%g", calc(pos1, pos2)); //���������
        if(i<num_Eqs2-1)   printf(",");
    }
    printf("\n");
    return 0;
}
