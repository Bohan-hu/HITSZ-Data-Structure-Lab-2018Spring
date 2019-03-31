#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100
char fileadd[1000] = "test4.txt";
int xmax, ymax;
int count = 0;
typedef enum {  //�����������
    WALL, AVAILABLE
} pointType;
typedef enum { //�������״̬
    UNVISITED, VISITED
} Status;

typedef struct _point { //����"��"��ṹ��
    int x, y; //�������
    pointType type; //������ͣ�ǽ����ͨ·��
    Status status; //״̬���Ƿ��Ѿ������ʣ�
} Point;

typedef struct _stack { //����ջ��ģ��ݹ����
    int top;
    Point route[10000000];
} Stack;


Stack *initStack() { //��ʼ��ջ
    Stack *ret = (Stack *) malloc(sizeof(Stack));
    ret->top = -1;
    return ret;
}

void push(Point a, Stack *S) { //��a����ջS
    S->top++;
    S->route[S->top] = a;
}

Point pop(Stack *S) { //��ջ��Ԫ�ص���
    S->top--;
    return S->route[S->top + 1];
}

Point top(Stack *S) { //����ջ��Ԫ��
    return S->route[S->top];
}


Point nextNbr(Point map[MAX_SIZE][MAX_SIZE], Point A) { //Ѱ����һ��δ�����ʵ��ھ�
    int next[4][2] = {{0,  1},
                      {1,  0},
                      {0,  -1},
                      {-1, 0}}; //�ĸ�������̽
    Point nextP;
    Point noTAG; //��û���ھӽڵ�ʱ������һ���ض���ֵ(-1,-1)
    noTAG.x = -1;
    noTAG.y = -1;
    for (int i = 0; i < 4; i++) { //���ĸ�������̽
        nextP.x = A.x + next[i][0];
        nextP.y = A.y + next[i][1];
        if (nextP.x >= 0 && nextP.y >= 0 && nextP.x < xmax && nextP.y < ymax && //���ж��Ƿ�Խ�磬���ܽ�����һ���жϣ���·���ԣ�
            map[nextP.x][nextP.y].status == UNVISITED  //δ������
            && map[nextP.x][nextP.y].type == AVAILABLE) { //�����ϰ�
            return nextP; //���Է���
        }
    }
    return noTAG; //���е��ھӽڵ��Ѿ����ʹ����򲻴����ھӽڵ㣬�������
}


int findPoints(Point map[MAX_SIZE][MAX_SIZE], Point start) {
    int count = 0;
    Stack *S = initStack();
    push(start, S); //�����ջ
    while (S->top > -1) { //��ջ�ǿ�ʱ
        Point cur = top(S);
        //Ѱ��ջ���ڵ����һ���ھ�
        cur = nextNbr(map, cur);
        if (cur.x == -1 && cur.y == -1) {   //�Ѿ���·����
            Point tmp = pop(S); //������ջ
            //printf("%d,%d poped\n", tmp.x, tmp.y);
        } else {
            push(cur, S);
            //printf("%d,%d pushed\n", cur.x, cur.y);
            count++; //����������1
            map[cur.x][cur.y].status = VISITED; //��Ƕ���
        }
    }
    return count - 1; //���س�ȥ���ɴ����Ŀ
}


int main(void) {
    printf("Number:SZ170110113\nName:Hubohan\nLab2_Task1_Stack\n");
    printf(fileadd);
    printf("\n");
    Point map[MAX_SIZE][MAX_SIZE];
    char _map[MAX_SIZE][MAX_SIZE];
    Point start;
    int i, j;
    //�ض����������������ͼ
    freopen(fileadd, "r", stdin);
    scanf("%d", &xmax);
    scanf("%d", &ymax);
    getchar();
    for (i = 0; i < xmax; i++) {
        for (j = 0; j < ymax; j++) {
            scanf("%c", &_map[i][j]);
            map[i][j].status = UNVISITED;
            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].type = (_map[i][j] == '#' ? WALL : AVAILABLE);
            if (_map[i][j] == '?') {
                start = map[i][j];
            }
        }
        getchar();
    }
    //�����ͼ����ȷ��
    /* for (i = 0; i < xmax; i++) {
         for (j = 0; j < ymax; j++) {
             //printf("%d",_map[i][j]);
             if (map[i][j].type == AVAILABLE) printf("1");
             else printf("0");
         }
         printf("\n");
     }*/

    int total = findPoints(map, start);
    printf("Start From: (%d,%d)\n", start.x, start.y);
    printf("Total:%d\n", total);
    freopen("CON", "r", stdin);
    system("pause");
    return 0;
}
