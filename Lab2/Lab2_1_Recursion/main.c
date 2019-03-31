#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100
char fileadd[1000] = "test1.txt";
int xmax, ymax;
int count = 0;
typedef enum { //定义格点的类型
    WALL, AVAILABLE
} pointType;
typedef enum { //定义格点的状态
    UNVISITED, VISITED
} Status;

typedef struct _point { //定义"点"类结构体
    int x, y; //点的坐标
    pointType type; //点的类型（墙还是通路）
    Status status;
} Point;

Point nextNbr(Point map[MAX_SIZE][MAX_SIZE], Point A) { //寻找下一个未被访问的邻居
    int next[4][2] = {{0,  1},
                      {1,  0},
                      {0,  -1},
                      {-1, 0}}; //四个方向试探
    Point nextP;
    Point noTAG;  //当没有邻居节点时，返回一个特定的坐标值（-1，-1）
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


void findPoints(Point map[MAX_SIZE][MAX_SIZE], Point start, int *count) {
    Point cur = nextNbr(map, start);  //寻找传入节点的下一个节点
    while (cur.x != -1 && cur.y != -1) { //如果该节点存在可以访问的邻居节点
        *count = *count + 1; //计数变量增加1
        map[cur.x][cur.y].status = VISITED; //标记已访问，以防重复
        findPoints(map, cur, count); //以当前节点为起点，进行下一轮搜索
        cur = nextNbr(map, start); //沿某个方向搜搜完毕，换一个新的邻居
    }
    return;
}


int main(void) {
    printf("Number:SZ170110113\nName:Hubohan\nLab2_Task1_Recursive\n");
    int count = 0;
    Point map[MAX_SIZE][MAX_SIZE];
    char _map[MAX_SIZE][MAX_SIZE];
    Point start;
    int i, j;
    //重定向输入流，读入地图
    freopen(fileadd, "r", stdin);
    scanf("%d", &xmax);
    scanf("%d", &ymax);
    getchar(); //处理回车
    for (i = 0; i < xmax; i++) {
        for (j = 0; j < ymax; j++) {
            scanf("%c", &_map[i][j]);
            map[i][j].status = UNVISITED;
            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].type = (_map[i][j] == '#' ? WALL : AVAILABLE);
            if (_map[i][j] == '?') {
                start = map[i][j]; //记录起点
            }
        }
        getchar(); //处理回车
    }
    //输出地图进行确认
    for (i = 0; i < xmax; i++) {
        for (j = 0; j < ymax; j++) {
            printf("%c", _map[i][j]);
        }
        printf("\n");
    }
    findPoints(map, start, &count);
    count--; //由于递归调用会使起点自身被算入，所以需要减去1
    printf("Start From: (%d,%d)\n", start.x, start.y);
    printf("Total:%d\n", count);
    freopen("CON", "r", stdin); //将输入流重定向回控制台
    system("pause");
    return 0;
}
