#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100
char fileadd[1000] = "test4.txt";
int xmax, ymax;
int count = 0;
typedef enum {  //定义格点的类型
    WALL, AVAILABLE
} pointType;
typedef enum { //定义格点的状态
    UNVISITED, VISITED
} Status;

typedef struct _point { //定义"点"类结构体
    int x, y; //点的坐标
    pointType type; //点的类型（墙还是通路）
    Status status; //状态（是否已经被访问）
} Point;

typedef struct _stack { //工作栈，模拟递归调用
    int top;
    Point route[10000000];
} Stack;


Stack *initStack() { //初始化栈
    Stack *ret = (Stack *) malloc(sizeof(Stack));
    ret->top = -1;
    return ret;
}

void push(Point a, Stack *S) { //将a推入栈S
    S->top++;
    S->route[S->top] = a;
}

Point pop(Stack *S) { //将栈顶元素弹出
    S->top--;
    return S->route[S->top + 1];
}

Point top(Stack *S) { //返回栈顶元素
    return S->route[S->top];
}


Point nextNbr(Point map[MAX_SIZE][MAX_SIZE], Point A) { //寻找下一个未被访问的邻居
    int next[4][2] = {{0,  1},
                      {1,  0},
                      {0,  -1},
                      {-1, 0}}; //四个方向试探
    Point nextP;
    Point noTAG; //当没有邻居节点时，返回一个特定的值(-1,-1)
    noTAG.x = -1;
    noTAG.y = -1;
    for (int i = 0; i < 4; i++) { //向四个方向试探
        nextP.x = A.x + next[i][0];
        nextP.y = A.y + next[i][1];
        if (nextP.x >= 0 && nextP.y >= 0 && nextP.x < xmax && nextP.y < ymax && //先判断是否越界，才能进行下一个判断（短路特性）
            map[nextP.x][nextP.y].status == UNVISITED  //未被访问
            && map[nextP.x][nextP.y].type == AVAILABLE) { //不是障碍
            return nextP; //可以访问
        }
    }
    return noTAG; //所有的邻居节点已经访问过，或不存在邻居节点，必须回溯
}


int findPoints(Point map[MAX_SIZE][MAX_SIZE], Point start) {
    int count = 0;
    Stack *S = initStack();
    push(start, S); //起点入栈
    while (S->top > -1) { //当栈非空时
        Point cur = top(S);
        //寻找栈顶节点的下一个邻居
        cur = nextNbr(map, cur);
        if (cur.x == -1 && cur.y == -1) {   //已经无路可走
            Point tmp = pop(S); //顶点退栈
            //printf("%d,%d poped\n", tmp.x, tmp.y);
        } else {
            push(cur, S);
            //printf("%d,%d pushed\n", cur.x, cur.y);
            count++; //计数变量加1
            map[cur.x][cur.y].status = VISITED; //标记顶点
        }
    }
    return count - 1; //返回除去起点可达点数目
}


int main(void) {
    printf("Number:SZ170110113\nName:Hubohan\nLab2_Task1_Stack\n");
    printf(fileadd);
    printf("\n");
    Point map[MAX_SIZE][MAX_SIZE];
    char _map[MAX_SIZE][MAX_SIZE];
    Point start;
    int i, j;
    //重定向输入流，读入地图
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
    //输出地图进行确认
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
