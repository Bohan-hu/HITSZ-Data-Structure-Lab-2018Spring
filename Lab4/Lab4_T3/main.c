#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define isDigit(x)  ( '0'<=(x) && (x)<='9' )
typedef int BusNum;
#define ST_LENGTH 1000
#define MAX_BUS 25
#define MAX_STATION 100
#define WEIGHT 1
typedef struct _route { //Dijkstra�㷨�����ڼ�¼·���Ľṹ��
    char route[ST_LENGTH]; //��ǰ��·��
    int step; //·������
} Route;

//�洢�ṹ���ڽӱ�
typedef struct _Arc {
    BusNum buses[MAX_BUS];//�����ö���Ĺ�����·��
    int bus_num;//�����û��Ĺ���������Ϊ0�����������վ��û��ֱ�
} Arc;
typedef struct _Vnode {
    char name[ST_LENGTH]; //����վ��
    //Arc *firstArc; //��һ����֮������Ķ���
    BusNum buses[MAX_BUS];//�����ö���Ĺ�����·��
    int bus_num;//�����ö���Ĺ���������Ϊ0�����������վ��û��ֱ�
    int nbr[MAX_BUS][3];//ֱ��·���ϵ�ǰ��վ���
} Vertex;
typedef struct {
    Vertex info_Station[MAX_STATION]; //�洢�������Ϣ
    int ver_num; //�����������ߵ�����
    Arc routes[MAX_STATION][MAX_STATION];//�洢������Ϣ���ڽӾ���
} Graph;


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


int locate_Station(Graph *G, char name[]) { //��λĳһ����վ����
    int i;
    for (i = 0; i < G->ver_num; i++) {
        if (strcmp(name, G->info_Station[i].name) == 0) return i; //���У�����λ��
    }
    return -1;//��û�У�����-1
}


void clear_map(Graph *map) { //��ʼ����ͼ�ṹ��
    int i, j;
    for (i = 0; i < MAX_STATION; i++) {
        for (j = 0; j < MAX_STATION; j++) {
            map->routes[i][j].bus_num = 0; //����
            memset(map->routes[i][j].buses, 0, sizeof(map->routes[i][j].buses)); //����
        }
        map->info_Station[i].bus_num = 0;
        memset(map->info_Station[i].buses, 0, sizeof(map->info_Station[i].buses));
    }
    map->ver_num = 0;
}

//�ж��Ƿ�ֱ�����ֱ�����ֱ��·��
int direct_arr(Graph *map, char *src, char *dst) {
    int loc_src = locate_Station(map, src);
    int loc_dst = locate_Station(map, dst);
    if (loc_dst == -1 || loc_src == -1) { //��վ�����ڣ�����-2
        printf("δ�ҵ���س�վ��\n");
        return -2;
    }
    for (int i = 0; i < map->info_Station[loc_src].bus_num; i++) {
        for (int j = 0; j < map->info_Station[loc_dst].bus_num; j++) {
            if (map->info_Station[loc_src].buses[i] == map->info_Station[loc_dst].buses[j]) {
                return map->info_Station[loc_src].buses[i]; //���ԱȽ�������վ���Ƿ�����ͬ·��
            }
        }
    }
    return -1;
}

void printMap(Graph *map) {
    for (int i = 0; i < map->ver_num; i++) {
        printf("%d:%s\t", i, map->info_Station[i].name);
        printf("ǰ����%d,��̣�%d\n", map->info_Station[i].nbr[0][0], map->info_Station[i].nbr[0][1]);
    }
    for (int i = 0; i < map->ver_num; i++) {
        printf("%d  |  ", i);
        for (int j = 0; j < map->ver_num; j++) {
            printf("%d ", map->routes[i][j].bus_num);
        }
        printf("\n");
    }
    printf("����%d����վ��\n", map->ver_num);
}

void addLine(Graph *map, char *line) { //�����·
    char *curr = line;
    //��ȡ������·��
    int linenum;
    linenum = readDigit(&curr);
    //��·�ݴ�
    char *temp[MAX_STATION];
    int t_Size = 0;
    //�ָ���������
    curr = strtok(curr, "-");
    while (curr != NULL) {
        temp[t_Size] = curr;
        t_Size++; //��������վ����
        curr = strtok(NULL, "-");
    }
    int former_num = -1;
    //ע�⣺����δ������ֹʱ���������ڵ��ַ��������
    char *sta1, *sta2;
    int pos1;
    int pos2;
    for (int i = 0; i < t_Size - 1; i++) {
        //��ȡ���ڵĳ�վ
        sta1 = temp[i];
        sta2 = temp[i + 1];
        pos1 = locate_Station(map, sta1);
        pos2 = locate_Station(map, sta2);
        //��վ�����ڣ������
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
        //����������ͼ������˫��Ҫ��¼



        map->routes[pos1][pos2].buses[map->routes[pos1][pos2].bus_num] = linenum; //�ڱ�������һ��������·��Ϣ
        map->routes[pos1][pos2].bus_num++; //�ɴ﹫����·����1

        map->routes[pos2][pos1].buses[map->routes[pos2][pos1].bus_num] = linenum; //�ڱ�������һ��������·��Ϣ
        map->routes[pos2][pos1].bus_num++; //�ɴ﹫����·����1

        map->info_Station[pos1].buses[map->info_Station[pos1].bus_num] = linenum; //�ڶ�������Ҳ��¼������·
        map->info_Station[pos1].nbr[map->info_Station[pos1].bus_num][0] = former_num; //����·�ϵ�ǰ��
        map->info_Station[pos1].nbr[map->info_Station[pos1].bus_num][1] = pos2; //����·�ϵĺ��
        map->info_Station[pos1].nbr[map->info_Station[pos1].bus_num][2] = linenum; //����·�ϵĺ��
        map->info_Station[pos1].bus_num++;

        former_num = pos1;
    }
    map->info_Station[pos2].buses[map->info_Station[pos2].bus_num] = linenum;
    map->info_Station[pos2].nbr[map->info_Station[pos2].bus_num][0] = pos1; //����·�ϵ�ǰ��
    map->info_Station[pos2].nbr[map->info_Station[pos2].bus_num][1] = -1; //����·�ϵĺ��
    map->info_Station[pos2].nbr[map->info_Station[pos2].bus_num][2] = linenum; //����·�ϵĺ��
}

void readFileInfo(Graph *map, char *fname) {
    char line[ST_LENGTH];
    //���ļ�
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        printf("�ļ�%s�����ڣ����ֶ�¼��!\n", fname);
        return;
    }
    printf("���ļ���ȡ���ݳɹ���\n");
    //������Ϣ
    do {
        fscanf(fp, "%s", line);
        //printf("%s\n", line);
        addLine(map, line);
    } while (getc(fp) != EOF);

}


Route *short_Path(Graph *map, char *csrc, char *cdst) { //Dijkstra�㷨Ѱ�����·
    Route *ret = (Route *) malloc(sizeof(Route) * map->ver_num);
    //˼·���ȹ��죬�ٲ��
    int src = locate_Station(map, csrc);
    int dst = locate_Station(map, cdst);
    if (dst == -1 || src == -1) { //û���ҵ���վ
        printf("δ�ҵ���س�վ��\n");
        return NULL;
    }
    int mark[MAX_STATION]; //�����ǣ����߻�ͷ·
    int min;
    int minPos = 0;
    int count = 0;
    memset(mark, 0, sizeof(mark)); //��ʼ��
    int i;
    //��ʼ������
    for (i = 0; i < map->ver_num; i++) {
        if (map->routes[src][i].bus_num != 0) {
            ret[i].step = WEIGHT; //��ʼ������������ÿһ���ɴﶥ��Ϊ·�߳�Ϊ1
        } else {
            ret[i].step = 999; //���ɴﶥ������Ϊ����
        }
        char lines[100] = "";
        char tmp[10] = "";
        for (int j = map->routes[src][i].bus_num - 1; j >= 0; j--) {
            //��¼���������õ��·����Ϣ���������������֮�乫����·��
            if (j == 0) {
                sprintf(tmp, "%d", map->routes[src][i].buses[j]);
            } else {
                sprintf(tmp, "%d/", map->routes[src][i].buses[j]);
            }
            strcat(lines, tmp); //�����ַ���
        }
        sprintf(ret[i].route, "%s--%s-->%s", map->info_Station[src].name, lines,
                map->info_Station[i].name);  //��ԭ�ȵ�վ������Ӹ��Գ�վ��
    }

    //Ԥ������ϣ���ʼ�������㷨
    ret[src].step = 0; //������Ϊ0
    mark[src] = 1; //������Ϊ���ñ��

    while (count < map->ver_num) {
        min = 999;//ÿ�ζ�Ӧ������һ��
        for (int i = 0; i < map->ver_num; i++) { //����·���о�����С�ĵ�
            if (ret[i].step < min && mark[i] == 0) {
                min = ret[i].step;
                minPos = i;
            }
        }
        mark[minPos] = 1; //���ñ��
        count++;
        for (int i = 0; i < map->ver_num; i++) { //����·���еı��
            if (mark[i] == 0 && map->routes[minPos][i].bus_num != 0 &&
                ret[minPos].step + WEIGHT < ret[i].step) { //��Ҫ����·��
                ret[i].step = ret[minPos].step + WEIGHT; //����·��
                //������¼
                char lines[100] = "";
                char tmp[10] = "";
                for (int j = map->routes[minPos][i].bus_num - 1; j >= 0; j--) { //�洢��һ����·��
                    if (j == 0) { //��ʱ�Ƚ����Ե������·����ַ���
                        sprintf(tmp, "%d", map->routes[minPos][i].buses[j]);
                    } else {
                        sprintf(tmp, "%d/", map->routes[minPos][i].buses[j]);
                    }
                    strcat(lines, tmp);
                }
                sprintf(ret[i].route, "%s--%s-->%s", ret[minPos].route, lines,
                        map->info_Station[i].name); //�����ַ���
            }
        }
    }
    if (ret[dst].step == 999) {
        printf("������·����\n");
        return NULL;
    }
    return &ret[dst];
}

int del_Station(Graph *map, char *name) { //ɾ��ĳ����վ
    int loc = locate_Station(map, name); //��λĳ����վ
    if (loc == -1) { //δ�ҵ���վ
        return -1;
    }
    int adj1, adj2;//����ֱ�������
    //int num_neigh = 0;
    //������·����
    for (int i = 0; i < map->info_Station[loc].bus_num; i++) {  //���ѭ������·��
        adj1 = map->info_Station[loc].nbr[i][0];
        adj2 = map->info_Station[loc].nbr[i][1];
        if (adj1 == -1 || adj2 == -1) {
            continue;
        }
        map->routes[adj1][adj2].buses[map->routes[adj1][adj2].bus_num++] = map->info_Station[loc].nbr[i][2];
        map->routes[adj2][adj1].buses[map->routes[adj2][adj1].bus_num++] = map->info_Station[loc].nbr[i][2];
    }


    //ɾ��������Ϣ
    map->info_Station[loc].bus_num = 0;
    for (int i = 0; i < map->ver_num; i++) { //ɾ����
        map->routes[loc][i].bus_num = 0;
        map->routes[i][loc].bus_num = 0;
    }
    strcpy(map->info_Station[loc].name, "");
    return 0;
}

int main() {
    //��ʼ���洢�ṹ
    Graph map;
    clear_map(&map);
    readFileInfo(&map, "data.txt"); //���ļ���ȡ����
    int choice;
    //printMap(&map);
    do { //��ʾ�˵�
        printf(
                "\n1.�ֶ�¼�뵥��������Ϣ\n"
                        "2.ɾ������վ��\n"
                        "3.��ѯ���·��\n"
                        "4.��ѯ�Ƿ�ֱ��\n"
                        "0.�˳�\n"
                        "���������ѡ��");
        while (scanf("%d", &choice) != 1) { //�����쳣����
            fflush(stdin);
            printf("�Ƿ����룬���������룡\n���������ѡ��");
        }
        char nline[ST_LENGTH];
        char csrc[ST_LENGTH];
        char cdst[ST_LENGTH];
        Route *r;
        switch (choice) {
            case 1: //�����·
                printf("������·�ߣ�");
                scanf("%s", nline);
                addLine(&map, nline);
                printf("��ӳɹ���\n");
                break;
            case 2: //ɾ��վ��
                printf("������Ҫɾ����վ�㣺");
                scanf("%s", nline);
                if (del_Station(&map, nline) == -1) { //δ�ҵ����վ��
                    printf("ɾ��ʧ�ܣ�δ�ҵ�վ�㣡\n");
                } else {
                    printf("ɾ���ɹ���\n");
                    // printMap(&map);
                }
                break;
            case 3: //��ѯ���·
                printf("���������վ��");
                scanf("%s", csrc);
                printf("�������յ�վ��");
                scanf("%s", cdst);
                r = short_Path(&map, csrc, cdst);
                if (r != NULL) { //��վ������
                    printf("���·����%d\n\n", r->step);
                    printf("���е�ĳ�����·��Ϊ:%s\n", r->route);
                }
                break;
            case 4: //��ѯֱ��
                printf("���������վ��");
                scanf("%s", csrc);
                printf("�������յ�վ��");
                scanf("%s", cdst);
                r = short_Path(&map, csrc, cdst);
                if (r) { //��վ������
                    int line = direct_arr(&map, csrc, cdst);
                    printf("�� %s �� %s ", csrc, cdst);
                    if (line != -1 && line != -2) { //����ֱ��
                        printf("����ֱ��·��%d·��\n", line);
                    } else { //������ֱ��
                        printf("������ֱ��·����");
                        printf("�ɰ�����·�����ˣ�\n");
                        printf("%s\n", r->route);
                        printf("���·����%d\n\n", r->step);
                    }
                }
                break;
//            case 5:
//                scanf("%s", nline);
//                int k = locate_Station(&map, nline);
//                printf("%s:", map.info_Station[k].name);
//                printf("������");
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
