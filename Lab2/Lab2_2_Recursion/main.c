#include <stdio.h>
#include <stdlib.h>

char fileadd[1000] = "test2.txt";
int xmax, ymax;
int step = 0;
#define Boundry(X) ((X).x>=0 && (X).y>=0 && (X).x<xmax && (X).y<ymax)
#define MAX_SIZE 100
typedef enum { //����������
    WALL, AVAILABLE
} pointType;
typedef enum { //������״̬���Ƿ񱻷��ʣ�
    UNVISITED, VISITED
} Status;
typedef enum {
    false, true
} bool;
typedef struct _point { //����������
    int x, y; //����
    int step; //��·���еĲ���
    pointType type; //������ͣ����ϰ�����ͨ·��
    Status status; //���״̬���Ƿ��Ѿ������ʣ�
} Point;

bool found = false; //��Ǳ��������Ƿ��ҵ�·��

Point nextNbr(Point map[MAX_SIZE][MAX_SIZE], Point A) { //Ѱ����һ��δ�����ʵ��ھ�
    int next[4][2] = {{0,  1},
                      {1,  0},
                      {0,  -1},
                      {-1, 0}}; //�ĸ�����
    Point nextP;
    Point noTAG; //���û���ҵ����Է��ʵ��ھӣ��򷵻��ض�����(-1,-1)
    noTAG.x = -1;
    noTAG.y = -1;
    for (int i = 0; i < 4; i++) {
        nextP.x = A.x + next[i][0];
        nextP.y = A.y + next[i][1];
        if (nextP.x >= 0 && nextP.y >= 0 && nextP.x < xmax && nextP.y < ymax && //û��Խ��
            map[nextP.x][nextP.y].status == UNVISITED  //δ������
            && map[nextP.x][nextP.y].type == AVAILABLE) { //�����ϰ�
            return nextP; //���Է���
        }
    }
    return noTAG; //���е��ھӽڵ��Ѿ����ʹ����򲻴����ھӽڵ㣬ֻ�ܻ���
}

void findRoute(Point map[MAX_SIZE][MAX_SIZE], Point start, Point end) {
    if (start.x == end.x && start.y == end.y) { //�ݹ�Ļ����������ж��Ƿ��Ѿ������յ�
        map[start.x][start.y].step = -1;
        found = true;
        return;
    }
    Point cur = start;
    cur = nextNbr(map, cur); //��λ������һ���ھ�
    if (cur.x == -1 && cur.y == -1) { //�ݹ�Ļ�������2���ж��Ƿ��Ѿ���·���ߣ��������
        map[cur.x][cur.y].step = -1; //������·���ϵı�ţ���ʾ�Ƴ���·��
        step--; //��������1
        return; //����
    } else {
        step++; //����ǰ��1
        map[cur.x][cur.y].step = (step % 9 == 0 ? 9 : step % 9);  //��·���ϵı��
        map[cur.x][cur.y].status = VISITED; //״̬��Ϊ"�ѷ���"
        findRoute(map, cur, end); //����̽��·��
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
    //�ض��������������ļ���ȡ����
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
    //�����ͼ
    /*  for (i = 0; i < xmax; i++) {
          for (j = 0; j < ymax; j++) {
              //printf("%d",_map[i][j]);
              if (map[i][j].type == AVAILABLE) printf("1");
              else printf("0");
          }
          printf("\n");
      }*/
    findRoute(map, start, end);
    if (found) { //����ҵ�·�������·��
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
    } else { //���û��
        printf("Impossible.\n");
    }
    //���������ض���ؿ���̨
    freopen("CON", "r", stdin);
    system("pause");
    return 0;
}
