#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_NUM 100
#define isDigit(x)  ( '0'<=(x) && (x)<='9' ) //宏定义判断是否为数字
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

bool larger(int a, int b) { //a+b形式拼接时，是否更大（比较函数）
    char ca[100], cb[100];
    sprintf(ca, "%d", a);
    sprintf(cb, "%d", b);
    int len_a = (int) strlen(ca);
    int len_b = (int) strlen(cb);
    unsigned long long aplusb = (unsigned long long) pow(10, len_b) * a + b; //a_b拼接的形式
    unsigned long long bplusa = (unsigned long long) pow(10, len_a) * b + a; //b_a拼接的形式
    return aplusb > bplusa;
}



bool bubble(int all[], int lo, int hi) { //冒泡排序的每一轮交换（区间左闭右开）
    bool ret = true;
    int tmp;

    do {
        if (!larger(all[lo], all[lo + 1])) { //对最高位进行比较
            ret = false;
            tmp = all[lo];
            all[lo] = all[lo + 1];
            all[lo + 1] = tmp;
        }
        lo++;
    } while (lo < hi - 1);
    return ret;
}

void BubbleSort(int all[], int lo, int hi) {
    while (!bubble(all, lo, hi--));
}


int main() {
    int total[MAX_NUM]; //存放数据

    //读取部分
    while(1) {
        int numbers = 0;
        char input[MAX_NUM];
        scanf("%s", input);
        //处理输入
        char *curr = input;
        while (*curr != '\0') {
            if (!isDigit(*curr)) { //指针移动到数字位置
                curr++;
                continue;
            }
            total[numbers++] = readDigit(&curr);
        }
        if(numbers==0){ //异常输入
            printf("Error\n");
            continue;
        }
        BubbleSort(total, 0, numbers);
        printf("MAX:");
        for (int i = 0; i < numbers; i++) {
            printf("%d", total[i]);
        }
    printf("\n");
    }
    return 0;
}
