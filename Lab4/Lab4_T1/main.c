#include <stdio.h>
#include <string.h>
#define MAX_VERT 100
#define isDigit(x)  ( '0'<=(x) && (x)<='9' )
//4,[[1,0],[2,0],[3,1],[3,2]]
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

int main(void) {
    //定义变量
    int AdjMatrix[MAX_VERT][MAX_VERT]; //邻接矩阵
    int isVisited[MAX_VERT]; //访问标记
    int inDegree[MAX_VERT]; //入度列
    int num_ver=0; //顶点的个数
    int Stack[MAX_VERT]; //存放顶点的栈
    int Stack_top = -1; //栈顶标记

    //初始化
    memset(AdjMatrix,0, sizeof(AdjMatrix));
    memset(isVisited,0, sizeof(isVisited));
    memset(inDegree,0, sizeof(inDegree));
    memset(Stack,0, sizeof(Stack));

    char input[100] = "3,[[1,0],[2,1],[0,2]]";


    char *curr = input;
    //printf("%s\n",input);
    scanf("%s",input);
    num_ver = readDigit(&curr);

    int from, to; //有向边的头和尾
    int exit = 0; //字符串终止标记

    //读取输入序列
    while (*curr != '\0' && !exit) {

        while (!isDigit(*curr)) {
            curr++;
            if (*curr == '\0') { //字符串终止
                exit = 1;
                break;
            }
        } //定位到下一个数字
        if (exit == 1) break;
        to = readDigit(&curr); //读取数字

        while (!isDigit(*curr)) {
            curr++;
            if (*curr == '\0') { //字符串终止
                exit = 1;
                break;
            }
        } //定位到下一个数字
        if (exit == 1) break;
        from = readDigit(&curr); //读取数字

        AdjMatrix[from][to] = 1; //存入矩阵
        inDegree[to]++; //更新顶点入度
    }

    int count = 0; //计数变量
    int zero_in = 0;  //入度为0的顶点下标
    while (count < num_ver) {
        int i;
        for (i = 0; i < num_ver; i++) {
            if (inDegree[i] == 0 && !isVisited[i]) {//  找到入度为0的顶点
                zero_in = i;
                isVisited[i] = 1;
                break;
            }
        }
        if(i==num_ver){ //未找到入度为0的顶点，无法排序
            printf("图中存在环，无法拓扑排序。\n");
            return 0;
        }
        Stack[++Stack_top] = zero_in;//入栈

        //更新相邻顶点入度
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
