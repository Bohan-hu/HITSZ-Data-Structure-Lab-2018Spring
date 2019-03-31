#include <stdio.h>
#include <stdlib.h>
char fileadd[1000] = "test2.txt";
int xmax, ymax;
#define Boundry(X) ((X).x>=0 && (X).y>=0 && (X).x<xmax && (X).y<ymax)
#define MAX_SIZE 100
typedef enum { //定义格点类型（是否为障碍）
    WALL, AVAILABLE
} pointType;
typedef enum { //定义格点状态（是否被访问）
    UNVISITED, VISITED
} Status;
typedef enum {
    false, true
} bool;
typedef struct _point { //定义"坐标"类
    int x, y; //点的坐标
    int step; //在路径上的标号
    pointType type; //点的类型
    Status status; //点的状态
} Point;
typedef struct _stack { //工作栈
    int top;
    Point route[100000];
} Stack;


Stack *initStack() { //初始化一个空栈
    Stack *ret = (Stack *) malloc(sizeof(Stack));
    ret->top = -1;
    return ret;
}

void push(Point a, Stack *S) { //将a推入S
    S->top++;
    S->route[S->top] = a;
}

Point pop(Stack *S) { //将S栈顶元素弹出并返回
    S->top--;
    return S->route[S->top + 1];
}

Point top(Stack *S) { //返回S栈顶元素
    return S->route[S->top];
}


Point nextNbr(Point map[MAX_SIZE][MAX_SIZE], Point A) { //寻找下一个未被访问的邻居
    int next[4][2] = {{0,  1},
                      {1,  0},
                      {0,  -1},
                      {-1, 0}}; //四个方向枚举
    Point nextP;
    Point noTAG; //若找不到可以访问的邻接节点，返回特定坐标（-1，-1）
    noTAG.x = -1;
    noTAG.y = -1;
    for (int i = 0; i < 4; i++) {
        nextP.x = A.x + next[i][0];
        nextP.y = A.y + next[i][1];
        if (nextP.x >= 0 && nextP.y >= 0 && nextP.x < xmax && nextP.y < ymax && //没有越界
            map[nextP.x][nextP.y].status == UNVISITED  //未被访问
            && map[nextP.x][nextP.y].type == AVAILABLE) { //不是障碍
            return nextP; //可以访问
        }
    }
    return noTAG; //所有的邻居节点已经访问过，或不存在邻居节点
}

bool findRoute(Point map[MAX_SIZE][MAX_SIZE], Point start, Point end) {
    int step = 0; //步数记录
    Stack *S = initStack();
    push(start, S); //起点入栈
    while (S->top > -1) {
        Point cur = top(S);
        //寻找栈顶节点的下一个邻居
        cur = nextNbr(map, cur); //（之前错误把这一句写到了循环体外面）
        if (cur.x == end.x && cur.y == end.y) return true; //到达了终点
        if (cur.x == -1 && cur.y == -1) {   //已经无路可走
            Point tmp = pop(S); //顶点退栈
            //printf("step:%d:%d,%d poped\n", step, tmp.x, tmp.y);
            map[tmp.x][tmp.y].step = -1; //恢复标记，移除出路径
            step--;//回溯步数
        } else {
            push(cur, S); //将当前顶点纳入路径
            //printf("step%d:%d,%d pushed\n", step, cur.x, cur.y);
            map[cur.x][cur.y].status = VISITED; //标记顶点
            step++; //向前走一步
            map[cur.x][cur.y].step = (step%9==0?9:step%9); //标记顶点在你路径中的标号
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
    //重定向输入流，读取地图
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
    if (possible == false) { //如果无法到达
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
    //将输入流还原至控制台
    freopen("CON","r",stdin);
    system("pause");
    return 0;
}
