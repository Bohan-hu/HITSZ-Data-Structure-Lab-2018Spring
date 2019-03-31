#include <stdio.h>
#include <string.h>
#define MAX_VERT 100
#define isDigit(x)  ( '0'<=(x) && (x)<='9' )
//4,[[1,0],[2,0],[3,1],[3,2]]
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

int main(void) {
    //�������
    int AdjMatrix[MAX_VERT][MAX_VERT]; //�ڽӾ���
    int isVisited[MAX_VERT]; //���ʱ��
    int inDegree[MAX_VERT]; //�����
    int num_ver=0; //����ĸ���
    int Stack[MAX_VERT]; //��Ŷ����ջ
    int Stack_top = -1; //ջ�����

    //��ʼ��
    memset(AdjMatrix,0, sizeof(AdjMatrix));
    memset(isVisited,0, sizeof(isVisited));
    memset(inDegree,0, sizeof(inDegree));
    memset(Stack,0, sizeof(Stack));

    char input[100] = "3,[[1,0],[2,1],[0,2]]";


    char *curr = input;
    //printf("%s\n",input);
    scanf("%s",input);
    num_ver = readDigit(&curr);

    int from, to; //����ߵ�ͷ��β
    int exit = 0; //�ַ�����ֹ���

    //��ȡ��������
    while (*curr != '\0' && !exit) {

        while (!isDigit(*curr)) {
            curr++;
            if (*curr == '\0') { //�ַ�����ֹ
                exit = 1;
                break;
            }
        } //��λ����һ������
        if (exit == 1) break;
        to = readDigit(&curr); //��ȡ����

        while (!isDigit(*curr)) {
            curr++;
            if (*curr == '\0') { //�ַ�����ֹ
                exit = 1;
                break;
            }
        } //��λ����һ������
        if (exit == 1) break;
        from = readDigit(&curr); //��ȡ����

        AdjMatrix[from][to] = 1; //�������
        inDegree[to]++; //���¶������
    }

    int count = 0; //��������
    int zero_in = 0;  //���Ϊ0�Ķ����±�
    while (count < num_ver) {
        int i;
        for (i = 0; i < num_ver; i++) {
            if (inDegree[i] == 0 && !isVisited[i]) {//  �ҵ����Ϊ0�Ķ���
                zero_in = i;
                isVisited[i] = 1;
                break;
            }
        }
        if(i==num_ver){ //δ�ҵ����Ϊ0�Ķ��㣬�޷�����
            printf("ͼ�д��ڻ����޷���������\n");
            return 0;
        }
        Stack[++Stack_top] = zero_in;//��ջ

        //�������ڶ������
        int j;
        for (j = 0; j < num_ver; j++) {
            if (AdjMatrix[zero_in][j] == 1) {
                inDegree[j]--;
            }
        }
        count++;
    }


    for (int i = 0; i < num_ver; i++) {
        printf("%d ", Stack[i]);
    }
    printf("\n");
    return 0;
}
