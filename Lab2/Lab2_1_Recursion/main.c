#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100
char fileadd[1000] = "test1.txt";
int xmax, ymax;
int count = 0;
typedef enum { //�����������
    WALL, AVAILABLE
} pointType;
typedef enum { //�������״̬
    UNVISITED, VISITED
} Status;

typedef struct _point { //����"��"��ṹ��
    int x, y; //�������
    pointType type; //������ͣ�ǽ����ͨ·��
    Status status;
} Point;

Point nextNbr(Point map[MAX_SIZE][MAX_SIZE], Point A) { //Ѱ����һ��δ�����ʵ��ھ�
    int next[4][2] = {{0,  1},
                      {1,  0},
                      {0,  -1},
                      {-1, 0}}; //�ĸ�������̽
    Point nextP;
    Point noTAG;  //��û���ھӽڵ�ʱ������һ���ض�������ֵ��-1��-1��
    noTAG.x = -1;
    noTAG.y = -1;
    for (int i = 0; i < 4; i++) { //���ĸ�������̽
        nextP.x = A.x + next[i][0];
        nextP.y = A.y + next[i][1];
        if (nextP.x >= 0 && nextP.y >= 0 && nextP.x < xmax && nextP.y < ymax && //���ж��Ƿ�Խ�磬���ܽ�����һ���жϣ���·���ԣ�
            map[nextP.x][nextP.y].status == UNVISITED  //δ������
            && map[nextP.x][nextP.y].type == AVAILABLE) { //�����ϰ�
            return nextP; //���Է���
        }
    }
    return noTAG; //���е��ھӽڵ��Ѿ����ʹ����򲻴����ھӽڵ㣬�������
}


void findPoints(Point map[MAX_SIZE][MAX_SIZE], Point start, int *count) {
    Point cur = nextNbr(map, start);  //Ѱ�Ҵ���ڵ����һ���ڵ�
    while (cur.x != -1 && cur.y != -1) { //����ýڵ���ڿ��Է��ʵ��ھӽڵ�
        *count = *count + 1; //������������1
        map[cur.x][cur.y].status = VISITED; //����ѷ��ʣ��Է��ظ�
        findPoints(map, cur, count); //�Ե�ǰ�ڵ�Ϊ��㣬������һ������
        cur = nextNbr(map, start); //��ĳ������������ϣ���һ���µ��ھ�
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
    //�ض����������������ͼ
    freopen(fileadd, "r", stdin);
    scanf("%d", &xmax);
    scanf("%d", &ymax);
    getchar(); //����س�
    for (i = 0; i < xmax; i++) {
        for (j = 0; j < ymax; j++) {
            scanf("%c", &_map[i][j]);
            map[i][j].status = UNVISITED;
            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].type = (_map[i][j] == '#' ? WALL : AVAILABLE);
            if (_map[i][j] == '?') {
                start = map[i][j]; //��¼���
            }
        }
        getchar(); //����س�
    }
    //�����ͼ����ȷ��
    for (i = 0; i < xmax; i++) {
        for (j = 0; j < ymax; j++) {
            printf("%c", _map[i][j]);
        }
        printf("\n");
    }
    findPoints(map, start, &count);
    count--; //���ڵݹ���û�ʹ����������룬������Ҫ��ȥ1
    printf("Start From: (%d,%d)\n", start.x, start.y);
    printf("Total:%d\n", count);
    freopen("CON", "r", stdin); //���������ض���ؿ���̨
    system("pause");
    return 0;
}
