#include <stdio.h>
#include <stdlib.h>
char fileadd[1000] = "test2.txt";
int xmax, ymax;
#define Boundry(X) ((X).x>=0 && (X).y>=0 && (X).x<xmax && (X).y<ymax)
#define MAX_SIZE 100
typedef enum { //���������ͣ��Ƿ�Ϊ�ϰ���
    WALL, AVAILABLE
} pointType;
typedef enum { //������״̬���Ƿ񱻷��ʣ�
    UNVISITED, VISITED
} Status;
typedef enum {
    false, true
} bool;
typedef struct _point { //����"����"��
    int x, y; //�������
    int step; //��·���ϵı��
    pointType type; //�������
    Status status; //���״̬
} Point;
typedef struct _stack { //����ջ
    int top;
    Point route[100000];
} Stack;


Stack *initStack() { //��ʼ��һ����ջ
    Stack *ret = (Stack *) malloc(sizeof(Stack));
    ret->top = -1;
    return ret;
}

void push(Point a, Stack *S) { //��a����S
    S->top++;
    S->route[S->top] = a;
}

Point pop(Stack *S) { //��Sջ��Ԫ�ص���������
    S->top--;
    return S->route[S->top + 1];
}

Point top(Stack *S) { //����Sջ��Ԫ��
    return S->route[S->top];
}


Point nextNbr(Point map[MAX_SIZE][MAX_SIZE], Point A) { //Ѱ����һ��δ�����ʵ��ھ�
    int next[4][2] = {{0,  1},
                      {1,  0},
                      {0,  -1},
                      {-1, 0}}; //�ĸ�����ö��
    Point nextP;
    Point noTAG; //���Ҳ������Է��ʵ��ڽӽڵ㣬�����ض����꣨-1��-1��
    noTAG.x = -1;
    noTAG.y = -1;
    for (int i = 0; i < 4; i++) {
        nextP.x = A.x + next[i][0];
        nextP.y = A.y + next[i][1];
        if (nextP.x >= 0 && nextP.y >= 0 && nextP.x < xmax && nextP.y < ymax && //û��Խ��
            map[nextP.x][nextP.y].status == UNVISITED  //δ������
            && map[nextP.x][nextP.y].type == AVAILABLE) { //�����ϰ�
            return nextP; //���Է���
        }
    }
    return noTAG; //���е��ھӽڵ��Ѿ����ʹ����򲻴����ھӽڵ�
}

bool findRoute(Point map[MAX_SIZE][MAX_SIZE], Point start, Point end) {
    int step = 0; //������¼
    Stack *S = initStack();
    push(start, S); //�����ջ
    while (S->top > -1) {
        Point cur = top(S);
        //Ѱ��ջ���ڵ����һ���ھ�
        cur = nextNbr(map, cur); //��֮ǰ�������һ��д����ѭ�������棩
        if (cur.x == end.x && cur.y == end.y) return true; //�������յ�
        if (cur.x == -1 && cur.y == -1) {   //�Ѿ���·����
            Point tmp = pop(S); //������ջ
            //printf("step:%d:%d,%d poped\n", step, tmp.x, tmp.y);
            map[tmp.x][tmp.y].step = -1; //�ָ���ǣ��Ƴ���·��
            step--;//���ݲ���
        } else {
            push(cur, S); //����ǰ��������·��
            //printf("step%d:%d,%d pushed\n", step, cur.x, cur.y);
            map[cur.x][cur.y].status = VISITED; //��Ƕ���
            step++; //��ǰ��һ��
            map[cur.x][cur.y].step = (step%9==0?9:step%9); //��Ƕ�������·���еı��
            //if(cur.x==end.x && cur.y==end.y) return true;
        }
    }
    return false;
}


int main(void) {
    printf("Number:SZ170110113\nName:Hubohan\nLab2_Task2_Stack\n");
    printf(fileadd);
    printf("\n");
    Point map[MAX_SIZE][MAX_SIZE];
    char _map[MAX_SIZE][MAX_SIZE];
    Point start, end;
    //�ض�������������ȡ��ͼ
    freopen(fileadd, "r", stdin);
    scanf("%d", &xmax);
    scanf("%d", &ymax);
    // printf("xmax = %d,ymax = %d\n", xmax, ymax);
    int i, j;
    getchar();
    for (i = 0; i < xmax; i++) {
        for (j = 0; j < ymax; j++) {
            _map[i][j] = getchar();
            map[i][j].status = UNVISITED;
            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].step = -1;
            map[i][j].type = (_map[i][j] == '#' ? WALL : AVAILABLE);
            if (_map[i][j] == '?') {
                start = map[i][j];
            }
            if (_map[i][j] == '*') {
                end = map[i][j];
            }
        }
        getchar();
    }
    /*  for (i = 0; i < xmax; i++) {
          for (j = 0; j < ymax; j++) {
              //printf("%d",_map[i][j]);
              if (map[i][j].type == AVAILABLE) printf("1");
              else printf("0");
          }
          printf("\n");
      }*/
    bool possible = findRoute(map, start, end);
    if (possible == false) { //����޷�����
        printf("Impossible.\n");
    } else {
        for (i = 0; i < xmax; i++) {
            for (j = 0; j < ymax; j++) {
                //printf("%d",_map[i][j]);
                if (map[i][j].step != -1) {
                    printf("%d", map[i][j].step);
                    //printf("*");
                } else {
                    printf("%c", _map[i][j]);
                    //printf(" ");
                }
            }
            printf("\n");
        }
    }
    //����������ԭ������̨
    freopen("CON","r",stdin);
    system("pause");
    return 0;
}
