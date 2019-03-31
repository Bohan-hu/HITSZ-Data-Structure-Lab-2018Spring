#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_VerNUM 50
#define MAX_LENGTH 100

typedef struct exp {
    char dividend[MAX_LENGTH]; // 被除数
    char divisor[MAX_LENGTH]; //除数
    double result; //商
} Expression;
//数据结构设计：AdjMatrix[a][b]的值表示a/b的值

//设定为全局变量，方便函数进行参数调用
double AdjMatrix[MAX_VerNUM][MAX_VerNUM]; //邻接矩阵
char vertex[MAX_VerNUM][MAX_LENGTH]; //数和字母的对应关系
int num_Eqs; //已知方程式个数
int num_Eqs2; //未知待计算个数
int num_Verx; //顶点个数


int locate_Vex(char *str) { //在表格中定位数字
    for (int i = num_Verx - 1; i >= 0; i--) {
        if (strcmp(vertex[i], str) == 0)
            return i;
    }
    return -1;//没找到，返回-1
}

Expression *str2exp(char *str) { //解析表达式
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

void addEdge(Expression *exp) { //加入新边
    int pos_devidend, pos_devisor; //被除数和除数的位置
    double result = exp->result;
    //定位被除数和除数在矩阵中的编号
    pos_devidend = locate_Vex(exp->dividend);
    pos_devisor = locate_Vex(exp->divisor);
    //原先不存在，则添加
    if (pos_devidend == -1) { //添加顶点
        strcpy(vertex[num_Verx], exp->dividend);
        pos_devidend = num_Verx;
        num_Verx++;
    }
    if (pos_devisor == -1) { //添加顶点
        strcpy(vertex[num_Verx], exp->divisor);
        pos_devisor = num_Verx;
        num_Verx++;
    }

    //双向权重不同
    AdjMatrix[pos_devidend][pos_devisor] = result;
    AdjMatrix[pos_devisor][pos_devidend] = 1.0 / result;

}

int nextNbr(int isVisited[MAX_VerNUM][MAX_VerNUM],int row) {//寻找当前节点的第一个可以访问的邻居
    for (int i = num_Verx - 1; i >= 0; i--) {
        if (AdjMatrix[row][i] != 0 && !isVisited[row][i]) {  //没被访问过且连通
            isVisited[row][i] = 1;
            return i;
        }
    }
    return -1;
}

double calc(int from, int to) { //除法分解（路径转移算法）
    int isVisited[MAX_VerNUM][MAX_VerNUM]; //标记顶点是否访问
    memset(isVisited,0, sizeof(isVisited));
    double ret = 1;
    int Stack[MAX_VerNUM]; //存放除法计算的步骤
    int top = -1; //栈顶标记
    Stack[++top] = from;  //起点入栈
    while (top > -1) { //栈非空时
        from = nextNbr(isVisited,Stack[top]); //定位栈顶的下一个邻接节点
        if (from == -1) { //已经访问到所有的节点（死胡同）
            top--; //回溯
        } //如果没有找到可以使用的顶点，弹出栈顶
        else {
            Stack[++top] = from; //将找到的顶点入栈
            if (from == to) { //到达终点
                for (int i = 0; i < top; i++) { //计算结果
                    ret = ret * AdjMatrix[Stack[i]][Stack[i + 1]];
                }
                return ret;
            }
        }
    }
    return -1; //无法求计算结果，则返回-1
}

int main() {


    char input[MAX_LENGTH] = "a/b=0.4,b/d=2,c/a=4"; //输入条件
    char input_2[MAX_LENGTH] = "a/d,b/c"; //需要计算的式子
    char temp[MAX_VerNUM][MAX_LENGTH]; //存放分割后的表达式
    char temp_2[MAX_VerNUM][MAX_LENGTH]; //存放分割后需要计算的式子


    printf("请输入已知的表达式:");
    scanf("%s",input);
    //printf("%s\n",input);
    printf("请输入待计算的表达式:");
    scanf("%s",input_2);
    //printf("%s\n",input_2);

    //将多个表达式切割
    char *curr = input;
    curr = strtok(curr, ",");
    while (curr != NULL) {
        strcpy(temp[num_Eqs], curr);
        num_Eqs++; //输入的已知式子的个数
        curr = strtok(NULL, ",");
    }
    char *curr2 = input_2;
    curr2 = strtok(curr2, ",");
    while (curr2 != NULL) {
        strcpy(temp_2[num_Eqs2], curr2);
        num_Eqs2++; //输入的未知式子的个数
        curr2 = strtok(NULL, ",");
    }

    for (int i = 0; i < num_Eqs; i++) { //将表达式字符串解析后，加入邻接矩阵中
        //printf("解析后的表达式%d:", i);
        Expression *exp = str2exp(temp[i]); //将表达式字符串解析后存入结构体
        //printf("被除数：%s,除数:%s,商:%g\n", exp->dividend, exp->divisor, exp->result);
        addEdge(exp); //将解析后的表达式添加进图
    }

    // 输出表达式矩阵
    // printf("存入矩阵：\n");
    /*for (int i = 0; i < num_Verx; i++) {
        for (int j = 0; j < num_Verx; j++) {
            printf("%.3f   ", AdjMatrix[i][j]);
        }
        printf("\n");
    }*/

    for (int i = 0; i < num_Eqs2; i++) {
        //printf("解析后的计算式%d:", i);
        curr2 = temp_2[i];
        curr2 = strtok(curr2, "/");
        //printf("%s/", curr2);
        int pos1 = locate_Vex(curr2);//找出被除数
        curr2 = strtok(NULL, "");
        int pos2 = locate_Vex(curr2);//找出除数
        //printf("%s=", curr2);
        //printf("被除数：%s,除数:%s,商:%g\n", exp->dividend, exp->divisor, calc(exp->dividend,exp->divisor));
        printf("%g", calc(pos1, pos2)); //输出计算结果
        if(i<num_Eqs2-1)   printf(",");
    }
    printf("\n");
    return 0;
}
