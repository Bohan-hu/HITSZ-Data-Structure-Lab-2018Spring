#include <stdio.h>
#include <stdlib.h>

char fileadd[1000] = "test2.txt";
int xmax, ymax;
int step = 0;
#define Boundry(X) ((X).x>=0 && (X).y>=0 && (X).x<xmax && (X).y<ymax)
#define MAX_SIZE 100
typedef enum { //定义格点类型
    WALL, AVAILABLE
} pointType;
typedef enum { //定义格点状态（是否被访问）
    UNVISITED, VISITED
} Status;
typedef enum {
    false, true
} bool;
typedef struct _point { //定义点的类型
    int x, y; //坐标
    int step; //在路径中的步数
    pointType type; //点的类型（是障碍还是通路）
    Status status; //点的状态（是否已经被访问）
} Point;

bool found = false; //标记变量，看是否找到路径

Point nextNbr(Point map[MAX_SIZE][MAX_SIZE], Point A) { //寻找下一个未被访问的邻居
    int next[4][2] = {{0,  1},
                      {1,  0},
                      {0,  -1},
                      {-1, 0}}; //四个方向
    Point nextP;
    Point noTAG; //如果没有找到可以访问的邻居，则返回特定坐标(-1,-1)
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
    return noTAG; //所有的邻居节点已经访问过，或不存在邻居节点，只能回溯
}

void findRoute(Point map[MAX_SIZE][MAX_SIZE], Point start, Point end) {
    if (start.x == end.x && start.y == end.y) { //递归的基线条件：判断是否已经到达终点
        map[start.x][start.y].step = -1;
        found = true;
        return;
    }
    Point cur = start;
    cur = nextNbr(map, cur); //定位起点的下一个邻居
    if (cur.x == -1 && cur.y == -1) { //递归的基线条件2：判断是否已经无路可走，必须回溯
        map[cur.x][cur.y].step = -1; //重置在路径上的标号，表示移除出路径
        step--; //步数回退1
        return; //回溯
    } else {
        step++; //步数前进1
        map[cur.x][cur.y].step = (step % 9 == 0 ? 9 : step % 9);  //在路径上的标号
        map[cur.x][cur.y].status = VISITED; //状态置为"已访问"
        findRoute(map, cur, end); //继续探索路径
    }
    return;
}


int main(void) {
    printf("Number:SZ170110113\nName:Hubohan\nLab2_Task2_Recursive\n");
    printf(fileadd);
    printf("\n");
    Point map[MAX_SIZE][MAX_SIZE];
    char _map[MAX_SIZE][MAX_SIZE];
    Point start, end;
    //重定向输入流，从文件读取数据
    freopen(fileadd, "r", stdin);
    scanf("%d", &xmax);
    scanf("%d", &ymax);
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
    //输出地图
    /*  for (i = 0; i < xmax; i++) {
          for (j = 0; j < ymax; j++) {
              //printf("%d",_map[i][j]);
              if (map[i][j].type == AVAILABLE) printf("1");
              else printf("0");
          }
          printf("\n");
      }*/
    findRoute(map, start, end);
    if (found) { //如果找到路径，输出路径
        for (i = 0; i < xmax; i++) {
            for (j = 0; j < ymax; j++) {
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
    } else { //如果没有
        printf("Impossible.\n");
    }
    //将输入流重定向回控制台
    freopen("CON", "r", stdin);
    system("pause");
    return 0;
}
