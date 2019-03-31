#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define isDigit(x)  ( '0'<=(x) && (x)<='9' )
typedef int BusNum;
#define ST_LENGTH 1000
#define MAX_BUS 25
#define MAX_STATION 100
#define WEIGHT 1
typedef struct _route { //Dijkstra算法中用于记录路径的结构体
    char route[ST_LENGTH]; //当前点路径
    int step; //路径长度
} Route;

//存储结构：邻接表
typedef struct _Arc {
    BusNum buses[MAX_BUS];//经过该顶点的公交车路线
    int bus_num;//经过该弧的公交数量（为0，则代表两个站点没有直达）
} Arc;
typedef struct _Vnode {
    char name[ST_LENGTH]; //公交站名
    //Arc *firstArc; //第一条与之相关联的顶点
    BusNum buses[MAX_BUS];//经过该顶点的公交车路线
    int bus_num;//经过该顶点的公交数量（为0，则代表两个站点没有直达）
    int nbr[MAX_BUS][3];//直达路径上的前后站序号
} Vertex;
typedef struct {
    Vertex info_Station[MAX_STATION]; //存储顶点的信息
    int ver_num; //顶点数量、边的数量
    Arc routes[MAX_STATION][MAX_STATION];//存储弧的信息的邻接矩阵
} Graph;


int readDigit(char **c) { //读取操作数的函数（将会移动字符指针）
    //注意：这个函数对于字符指针做了移动，所以切记不能传入字符数组的首地址，否则将会丢失整个数据，应该传入字符指针的副本!!!
    int ret = 0;
    do {
        ret *= 10; //数字左移一位
        ret += **c - 48; //把个位加上来
        *c += sizeof(char); //字符指针前移一位
        //二级指针的运算问题？如果*c++,加的是sizeof(BT)，而此时的BT是？？？
    } while (isDigit(**c)); //在碰到下一个运算符之前
    return ret; //返回得到的操作数
}


int locate_Station(Graph *G, char name[]) { //定位某一公交站点编号
    int i;
    for (i = 0; i < G->ver_num; i++) {
        if (strcmp(name, G->info_Station[i].name) == 0) return i; //若有，返回位置
    }
    return -1;//若没有，返回-1
}


void clear_map(Graph *map) { //初始化地图结构体
    int i, j;
    for (i = 0; i < MAX_STATION; i++) {
        for (j = 0; j < MAX_STATION; j++) {
            map->routes[i][j].bus_num = 0; //清零
            memset(map->routes[i][j].buses, 0, sizeof(map->routes[i][j].buses)); //清零
        }
        map->info_Station[i].bus_num = 0;
        memset(map->info_Station[i].buses, 0, sizeof(map->info_Station[i].buses));
    }
    map->ver_num = 0;
}

//判断是否直达，若能直达，返回直达路径
int direct_arr(Graph *map, char *src, char *dst) {
    int loc_src = locate_Station(map, src);
    int loc_dst = locate_Station(map, dst);
    if (loc_dst == -1 || loc_src == -1) { //车站不存在，返回-2
        printf("未找到相关车站！\n");
        return -2;
    }
    for (int i = 0; i < map->info_Station[loc_src].bus_num; i++) {
        for (int j = 0; j < map->info_Station[loc_dst].bus_num; j++) {
            if (map->info_Station[loc_src].buses[i] == map->info_Station[loc_dst].buses[j]) {
                return map->info_Station[loc_src].buses[i]; //线性比较两个车站，是否有相同路线
            }
        }
    }
    return -1;
}

void printMap(Graph *map) {
    for (int i = 0; i < map->ver_num; i++) {
        printf("%d:%s\t", i, map->info_Station[i].name);
        printf("前驱：%d,后继：%d\n", map->info_Station[i].nbr[0][0], map->info_Station[i].nbr[0][1]);
    }
    for (int i = 0; i < map->ver_num; i++) {
        printf("%d  |  ", i);
        for (int j = 0; j < map->ver_num; j++) {
            printf("%d ", map->routes[i][j].bus_num);
        }
        printf("\n");
    }
    printf("共有%d个车站。\n", map->ver_num);
}

void addLine(Graph *map, char *line) { //添加线路
    char *curr = line;
    //读取公交线路号
    int linenum;
    linenum = readDigit(&curr);
    //线路暂存
    char *temp[MAX_STATION];
    int t_Size = 0;
    //分割输入序列
    curr = strtok(curr, "-");
    while (curr != NULL) {
        temp[t_Size] = curr;
        t_Size++; //输入序列站点数
        curr = strtok(NULL, "-");
    }
    int former_num = -1;
    //注意：程序未正常终止时，缓冲区内的字符不会输出
    char *sta1, *sta2;
    int pos1;
    int pos2;
    for (int i = 0; i < t_Size - 1; i++) {
        //读取相邻的车站
        sta1 = temp[i];
        sta2 = temp[i + 1];
        pos1 = locate_Station(map, sta1);
        pos2 = locate_Station(map, sta2);
        //车站不存在，则添加
        if (pos1 == -1) {
            strcpy(map->info_Station[map->ver_num].name, sta1);
            pos1 = map->ver_num;
            map->ver_num++;
        }
        if (pos2 == -1) {
            strcpy(map->info_Station[map->ver_num].name, sta2);
            pos2 = map->ver_num;
            map->ver_num++;
        }
        //由于是无向图，所以双向都要记录



        map->routes[pos1][pos2].buses[map->routes[pos1][pos2].bus_num] = linenum; //在边中增加一个公交线路信息
        map->routes[pos1][pos2].bus_num++; //可达公交线路增加1

        map->routes[pos2][pos1].buses[map->routes[pos2][pos1].bus_num] = linenum; //在边中增加一个公交线路信息
        map->routes[pos2][pos1].bus_num++; //可达公交线路增加1

        map->info_Station[pos1].buses[map->info_Station[pos1].bus_num] = linenum; //在顶点向量也记录公交线路
        map->info_Station[pos1].nbr[map->info_Station[pos1].bus_num][0] = former_num; //该线路上的前驱
        map->info_Station[pos1].nbr[map->info_Station[pos1].bus_num][1] = pos2; //该线路上的后继
        map->info_Station[pos1].nbr[map->info_Station[pos1].bus_num][2] = linenum; //该线路上的后继
        map->info_Station[pos1].bus_num++;

        former_num = pos1;
    }
    map->info_Station[pos2].buses[map->info_Station[pos2].bus_num] = linenum;
    map->info_Station[pos2].nbr[map->info_Station[pos2].bus_num][0] = pos1; //该线路上的前驱
    map->info_Station[pos2].nbr[map->info_Station[pos2].bus_num][1] = -1; //该线路上的后继
    map->info_Station[pos2].nbr[map->info_Station[pos2].bus_num][2] = linenum; //该线路上的后继
}

void readFileInfo(Graph *map, char *fname) {
    char line[ST_LENGTH];
    //打开文件
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        printf("文件%s不存在，请手动录入!\n", fname);
        return;
    }
    printf("从文件读取数据成功！\n");
    //读入信息
    do {
        fscanf(fp, "%s", line);
        //printf("%s\n", line);
        addLine(map, line);
    } while (getc(fp) != EOF);

}


Route *short_Path(Graph *map, char *csrc, char *cdst) { //Dijkstra算法寻找最短路
    Route *ret = (Route *) malloc(sizeof(Route) * map->ver_num);
    //思路：先构造，再查表
    int src = locate_Station(map, csrc);
    int dst = locate_Station(map, cdst);
    if (dst == -1 || src == -1) { //没有找到车站
        printf("未找到相关车站！\n");
        return NULL;
    }
    int mark[MAX_STATION]; //顶点标记，不走回头路
    int min;
    int minPos = 0;
    int count = 0;
    memset(mark, 0, sizeof(mark)); //初始化
    int i;
    //初始化工作
    for (i = 0; i < map->ver_num; i++) {
        if (map->routes[src][i].bus_num != 0) {
            ret[i].step = WEIGHT; //初始化从起点出发的每一个可达顶点为路线长为1
        } else {
            ret[i].step = 999; //不可达顶点设置为无穷
        }
        char lines[100] = "";
        char tmp[10] = "";
        for (int j = map->routes[src][i].bus_num - 1; j >= 0; j--) {
            //记录起点出发到该点的路线信息（这个部分是两点之间公交线路）
            if (j == 0) {
                sprintf(tmp, "%d", map->routes[src][i].buses[j]);
            } else {
                sprintf(tmp, "%d/", map->routes[src][i].buses[j]);
            }
            strcat(lines, tmp); //串接字符串
        }
        sprintf(ret[i].route, "%s--%s-->%s", map->info_Station[src].name, lines,
                map->info_Station[i].name);  //在原先的站后面添加各自车站名
    }

    //预处理完毕，开始进行主算法
    ret[src].step = 0; //起点距离为0
    mark[src] = 1; //标记起点为永久标号

    while (count < map->ver_num) {
        min = 999;//每次都应该重置一次
        for (int i = 0; i < map->ver_num; i++) { //查找路径中距离最小的点
            if (ret[i].step < min && mark[i] == 0) {
                min = ret[i].step;
                minPos = i;
            }
        }
        mark[minPos] = 1; //永久标号
        count++;
        for (int i = 0; i < map->ver_num; i++) { //更新路径中的标号
            if (mark[i] == 0 && map->routes[minPos][i].bus_num != 0 &&
                ret[minPos].step + WEIGHT < ret[i].step) { //需要更新路径
                ret[i].step = ret[minPos].step + WEIGHT; //更新路径
                //建立记录
                char lines[100] = "";
                char tmp[10] = "";
                for (int j = map->routes[minPos][i].bus_num - 1; j >= 0; j--) { //存储下一步的路径
                    if (j == 0) { //此时先将可以到达的线路存进字符串
                        sprintf(tmp, "%d", map->routes[minPos][i].buses[j]);
                    } else {
                        sprintf(tmp, "%d/", map->routes[minPos][i].buses[j]);
                    }
                    strcat(lines, tmp);
                }
                sprintf(ret[i].route, "%s--%s-->%s", ret[minPos].route, lines,
                        map->info_Station[i].name); //更新字符串
            }
        }
    }
    if (ret[dst].step == 999) {
        printf("两点无路径！\n");
        return NULL;
    }
    return &ret[dst];
}

int del_Station(Graph *map, char *name) { //删除某个车站
    int loc = locate_Station(map, name); //定位某个车站
    if (loc == -1) { //未找到车站
        return -1;
    }
    int adj1, adj2;//两条直达的名称
    //int num_neigh = 0;
    //逐条线路处理
    for (int i = 0; i < map->info_Station[loc].bus_num; i++) {  //外层循环迭代路线
        adj1 = map->info_Station[loc].nbr[i][0];
        adj2 = map->info_Station[loc].nbr[i][1];
        if (adj1 == -1 || adj2 == -1) {
            continue;
        }
        map->routes[adj1][adj2].buses[map->routes[adj1][adj2].bus_num++] = map->info_Station[loc].nbr[i][2];
        map->routes[adj2][adj1].buses[map->routes[adj2][adj1].bus_num++] = map->info_Station[loc].nbr[i][2];
    }


    //删除顶点信息
    map->info_Station[loc].bus_num = 0;
    for (int i = 0; i < map->ver_num; i++) { //删除边
        map->routes[loc][i].bus_num = 0;
        map->routes[i][loc].bus_num = 0;
    }
    strcpy(map->info_Station[loc].name, "");
    return 0;
}

int main() {
    //初始化存储结构
    Graph map;
    clear_map(&map);
    readFileInfo(&map, "data.txt"); //从文件读取数据
    int choice;
    //printMap(&map);
    do { //显示菜单
        printf(
                "\n1.手动录入单条公交信息\n"
                        "2.删除公交站点\n"
                        "3.查询最短路径\n"
                        "4.查询是否直达\n"
                        "0.退出\n"
                        "请输入你的选择：");
        while (scanf("%d", &choice) != 1) { //处理异常输入
            fflush(stdin);
            printf("非法输入，请重新输入！\n请输入你的选择：");
        }
        char nline[ST_LENGTH];
        char csrc[ST_LENGTH];
        char cdst[ST_LENGTH];
        Route *r;
        switch (choice) {
            case 1: //添加线路
                printf("请输入路线：");
                scanf("%s", nline);
                addLine(&map, nline);
                printf("添加成功！\n");
                break;
            case 2: //删除站点
                printf("请输入要删除的站点：");
                scanf("%s", nline);
                if (del_Station(&map, nline) == -1) { //未找到相关站点
                    printf("删除失败，未找到站点！\n");
                } else {
                    printf("删除成功！\n");
                    // printMap(&map);
                }
                break;
            case 3: //查询最短路
                printf("请输入起点站：");
                scanf("%s", csrc);
                printf("请输入终点站：");
                scanf("%s", cdst);
                r = short_Path(&map, csrc, cdst);
                if (r != NULL) { //车站都存在
                    printf("最短路径：%d\n\n", r->step);
                    printf("其中的某条最短路径为:%s\n", r->route);
                }
                break;
            case 4: //查询直达
                printf("请输入起点站：");
                scanf("%s", csrc);
                printf("请输入终点站：");
                scanf("%s", cdst);
                r = short_Path(&map, csrc, cdst);
                if (r) { //车站都存在
                    int line = direct_arr(&map, csrc, cdst);
                    printf("从 %s 到 %s ", csrc, cdst);
                    if (line != -1 && line != -2) { //存在直达
                        printf("存在直达路径%d路。\n", line);
                    } else { //不存在直达
                        printf("不存在直达路径，");
                        printf("可按以下路径换乘：\n");
                        printf("%s\n", r->route);
                        printf("最短路径：%d\n\n", r->step);
                    }
                }
                break;
//            case 5:
//                scanf("%s", nline);
//                int k = locate_Station(&map, nline);
//                printf("%s:", map.info_Station[k].name);
//                printf("经过：");
//                for (int i = 0; i < map.info_Station[k].bus_num; i++) {
//                    printf("%d", map.info_Station[k].buses[i]);
//                }
//                break;
            default:
                return 0;
        }
    } while (choice != 0);
    return 0;
}
