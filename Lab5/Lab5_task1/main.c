#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_NUM 100
#define isDigit(x)  ( '0'<=(x) && (x)<='9' ) //�궨���ж��Ƿ�Ϊ����
int readDigit(char **c) { //��ȡ�������ĺ����������ƶ��ַ�ָ�룩
    //ע�⣺������������ַ�ָ�������ƶ��������мǲ��ܴ����ַ�������׵�ַ�����򽫻ᶪʧ�������ݣ�Ӧ�ô����ַ�ָ��ĸ���!!!
    int ret = 0;
    do {
        ret *= 10; //��������һλ
        ret += **c - 48; //�Ѹ�λ������
        *c += sizeof(char); //�ַ�ָ��ǰ��һλ
        //����ָ����������⣿���*c++,�ӵ���sizeof(BT)������ʱ��BT�ǣ�����
    } while (isDigit(**c)); //��������һ�������֮ǰ
    return ret; //���صõ��Ĳ�����
}

bool larger(int a, int b) { //a+b��ʽƴ��ʱ���Ƿ���󣨱ȽϺ�����
    char ca[100], cb[100];
    sprintf(ca, "%d", a);
    sprintf(cb, "%d", b);
    int len_a = (int) strlen(ca);
    int len_b = (int) strlen(cb);
    unsigned long long aplusb = (unsigned long long) pow(10, len_b) * a + b; //a_bƴ�ӵ���ʽ
    unsigned long long bplusa = (unsigned long long) pow(10, len_a) * b + a; //b_aƴ�ӵ���ʽ
    return aplusb > bplusa;
}



bool bubble(int all[], int lo, int hi) { //ð�������ÿһ�ֽ�������������ҿ���
    bool ret = true;
    int tmp;

    do {
        if (!larger(all[lo], all[lo + 1])) { //�����λ���бȽ�
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
    int total[MAX_NUM]; //�������

    //��ȡ����
    while(1) {
        int numbers = 0;
        char input[MAX_NUM];
        scanf("%s", input);
        //��������
        char *curr = input;
        while (*curr != '\0') {
            if (!isDigit(*curr)) { //ָ���ƶ�������λ��
                curr++;
                continue;
            }
            total[numbers++] = readDigit(&curr);
        }
        if(numbers==0){ //�쳣����
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
