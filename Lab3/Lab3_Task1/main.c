#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define isLeaf(x) ( (x)->lChild == NULL && (x)->rChild == NULL ) //�ж��Ƿ�ΪҶ�ӽڵ�
#define MAX_SIZE 100

int max(int a, int b) { return a > b ? a : b; }

typedef struct bN {
    char data; //������
    struct bN *lChild; //����
    struct bN *rChild; //�Һ���
} binNode;

int depth(binNode *root) { //�������ĸ߶�
    if (root == NULL) return 0; //�����߶�Ϊ0
    else return (max(depth(root->lChild), depth(root->rChild)) + 1); //�ݹ鶨�壺���ĸ߶�Ϊ�����������߶�+1
}

binNode *initbinNode(char d) { //��ʼ���ڵ㣬ע�⣺����ֻ������Ҷ�ӽ�㣬���Բ���ȥ�ж�Union�������������
    binNode *ret = (binNode *) malloc(sizeof(binNode)); //����ռ�
    if (ret == NULL) exit(-1); //��ֹ���
    ret->data = d; //������ֵ
    ret->lChild = NULL;
    ret->rChild = NULL;
    return ret;
}

void deleteBinTree(binNode *root) { //���ٶ�����
    if (root == NULL) return; //�����սڵ�ֱ�ӷ���
    if (isLeaf(root)) { //Ҷ�ڵ��ͷź󷵻�
        free(root);
        return;
    } else {
        //�ݹ��ͷ���������
        deleteBinTree(root->lChild);
        deleteBinTree(root->rChild);
        return;
    }
}

char *cut(char *src, int start, int end) { //���ܣ���ȡ�ַ������ִ�
    char *ret = (char *) malloc(sizeof(char) * (end - start + 1));
    int i;
    for (i = 0; i < end - start; i++) ret[i] = src[start + i]; //���θ���
    ret[i] = '\0'; //��ֹ�ڴ����������ݴ���Ӱ�죬����һ��������
    return ret;
}

binNode *rebuildTree(char pre_Order[], char in_Order[]) {
    //��λ���ڵ�
    int length = strlen(in_Order); //���Ľڵ����
    if (length == 0) return NULL; //ƽ�����������
    binNode *ret = initbinNode(pre_Order[0]); //�������ڵ㣨����������еĵ�һ���ڵ㣩
    int i;
    int key;
    //��������ж�λ�����������ҵ�һ����Ҫ��Ȼ���ظ������������ɴ����з֣�
    for (i = 0; i < length; i++) {
        if (in_Order[i] == pre_Order[0]) { //
            key = i;
            break;
        }
    }
    if (i == length) { //�쳣��������Ҳ���������
        printf("Invalid Sequence!\n");
        exit(-1);
    }
    //�и���������������
    char *pre_Left = cut(pre_Order, 1, key + 1);
    char *pre_right = cut(pre_Order, key + 1, strlen(pre_Order));
    char *in_Left = cut(in_Order, 0, key);
    char *in_right = cut(in_Order, key + 1, strlen(in_Order));
    //�ݹ齨����������
    ret->lChild = rebuildTree(pre_Left, in_Left);
    ret->rChild = rebuildTree(pre_right, in_right);
    return ret;
}

void visit(binNode *bn) {
    printf("%c ", bn->data);
}

void traverse_Pre_re(binNode *root) { //�������
    if (root == NULL) {
        return;
    } else {
        visit(root);
        traverse_Pre_re(root->lChild); //����������
        traverse_Pre_re(root->rChild); //����������
    }
}

void traverse_In_re(binNode *root) {
    if (root == NULL) {
        return;
    } else {
        traverse_In_re(root->lChild); //����������
        visit(root);
        traverse_In_re(root->rChild); //����������
    }
}

void traverse_Pos_re(binNode *root) {
    if (root == NULL) {
        return;
    } else {
        traverse_Pos_re(root->lChild); //����������
        traverse_Pos_re(root->rChild); //����������
        visit(root);
    }
}


void printTree(binNode *root) {
    int dep = depth(root); //�������
    printf("\nDepth:%d\n", dep);
    binNode *level[1000] = {NULL,}; //��α���������
    int curr = 0; //��ͷ
    int tail = 0; //��β
    level[0] = root; //���ڵ����
    while (curr <= pow(2, dep) - 1) { //����һ�������������б���
        if (root != NULL) {
            level[++tail] = root->lChild; //���������
            level[++tail] = root->rChild; //���������
        } else { //�սڵ����������պ���
            level[++tail] = NULL;
            level[++tail] = NULL;
        }
        root = level[++curr]; //����һλ
    }

    int i, j, k;
    int endofLine[10]; //��i��Ӧ������Ľڵ�����
    int startPosofLine[10]; //ÿһ��Ӧ�ÿ�ʼ���������
    int margin[10]; //ÿһ���������ļ��
    for (i = 0; i < 10; i++) { //���������λ��
        endofLine[i] = (int) pow(2, i);
        startPosofLine[i] = (int) pow(2, dep - i - 1) - 1;
        margin[i] = (int) pow(2, dep - i) - 1;
    }
    int count = 0; //��������ĸ���
    for (i = 0; i < dep; i++) { //���ÿ��
        for (j = 0; j < startPosofLine[i]; j++) printf("   "); //����ƶ�����ʼλ��
        for (j = 0; j < endofLine[i]; j++) {
            if (level[count] == NULL) { //����ǿսڵ㣬����ո�
                printf("   ");
                for (k = 0; k < margin[i]; k++) printf("   "); //������
                count++;
                continue;
            } else { //������ǿսڵ㣬�������
                printf("%3c", level[count]->data);
                for (k = 0; k < margin[i]; k++) printf("   "); //������
                count++;
            }
        }
        printf("\n\n");
    }


}

bool isSym(binNode *root1, binNode *root2) {
    if (root1 == NULL && root2 == NULL) return true;//ƽ�����:���ǿսڵ�
    if (root1 == NULL || root2 == NULL) return false;//ƽ�������ֻ��һ��Ϊ��
    if (root1->data != root2->data) return false; //ƽ����������ݲ����
    if (root1->data == root2->data)
        return (isSym(root1->rChild, root2->lChild) && isSym(root1->lChild, root2->rChild));
    return true;//THE COMPILER WARNED, SHUT IT UP
}

int main() {
    //�����������
    char str[2 * MAX_SIZE];
    char preTravelOrder[MAX_SIZE];
    char inTravelOrder[MAX_SIZE];
    scanf("%s", str);
    //�и�����
    if(strchr(str,',')==NULL){
        printf("Invalid Sequence! Make sure to enter \��,\�� in English Mode!\n"); //������кϷ���
        exit(-1);
    }
    strcpy(preTravelOrder, strtok(str, ","));
    strcpy(inTravelOrder, strtok(NULL, ","));
    //ȷ�������Ϣ
    int length = strlen(preTravelOrder);
    printf("SeqLength:%d\nPre:%s\nIn:%s\n", length, preTravelOrder, inTravelOrder);
    //�ؽ�������
    binNode *newTree = rebuildTree(preTravelOrder, inTravelOrder);
    //�����������
    printf("Post Order Travel: ");
    traverse_Pos_re(newTree);
    printf("\n");
    //�������
    printTree(newTree);
    //�ж��Ƿ�Գ�
    printf("\n");
    if (isSym(newTree->lChild, newTree->rChild))
        printf("This is Symmetry.\n");
    else
        printf("This is NOT Symmetry.\n");
    return 0;
}
