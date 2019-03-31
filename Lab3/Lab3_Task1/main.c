#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define isLeaf(x) ( (x)->lChild == NULL && (x)->rChild == NULL ) //判断是否为叶子节点
#define MAX_SIZE 100

int max(int a, int b) { return a > b ? a : b; }

typedef struct bN {
    char data; //数据域
    struct bN *lChild; //左孩子
    struct bN *rChild; //右孩子
} binNode;

int depth(binNode *root) { //二叉树的高度
    if (root == NULL) return 0; //空树高度为0
    else return (max(depth(root->lChild), depth(root->rChild)) + 1); //递归定义：树的高度为左右子树最大高度+1
}

binNode *initbinNode(char d) { //初始化节点，注意：数字只存在于叶子结点，所以不用去判断Union里面的数据类型
    binNode *ret = (binNode *) malloc(sizeof(binNode)); //分配空间
    if (ret == NULL) exit(-1); //防止抽风
    ret->data = d; //数据域赋值
    ret->lChild = NULL;
    ret->rChild = NULL;
    return ret;
}

void deleteBinTree(binNode *root) { //销毁二叉树
    if (root == NULL) return; //碰到空节点直接返回
    if (isLeaf(root)) { //叶节点释放后返回
        free(root);
        return;
    } else {
        //递归释放左右子树
        deleteBinTree(root->lChild);
        deleteBinTree(root->rChild);
        return;
    }
}

char *cut(char *src, int start, int end) { //功能：截取字符串的字串
    char *ret = (char *) malloc(sizeof(char) * (end - start + 1));
    int i;
    for (i = 0; i < end - start; i++) ret[i] = src[start + i]; //依次复制
    ret[i] = '\0'; //防止内存中垃圾数据存在影响，创建一个结束符
    return ret;
}

binNode *rebuildTree(char pre_Order[], char in_Order[]) {
    //定位根节点
    int length = strlen(in_Order); //树的节点个数
    if (length == 0) return NULL; //平凡情况：空树
    binNode *ret = initbinNode(pre_Order[0]); //建立根节点（先序遍历序列的第一个节点）
    int i;
    int key;
    //中序遍历中定位树根（必须找第一个，要不然对重复的情况，会造成错误切分）
    for (i = 0; i < length; i++) {
        if (in_Order[i] == pre_Order[0]) { //
            key = i;
            break;
        }
    }
    if (i == length) { //异常情况处理（找不到树根）
        printf("Invalid Sequence!\n");
        exit(-1);
    }
    //切割左右子树的序列
    char *pre_Left = cut(pre_Order, 1, key + 1);
    char *pre_right = cut(pre_Order, key + 1, strlen(pre_Order));
    char *in_Left = cut(in_Order, 0, key);
    char *in_right = cut(in_Order, key + 1, strlen(in_Order));
    //递归建立左右子树
    ret->lChild = rebuildTree(pre_Left, in_Left);
    ret->rChild = rebuildTree(pre_right, in_right);
    return ret;
}

void visit(binNode *bn) {
    printf("%c ", bn->data);
}

void traverse_Pre_re(binNode *root) { //先序遍历
    if (root == NULL) {
        return;
    } else {
        visit(root);
        traverse_Pre_re(root->lChild); //遍历左子树
        traverse_Pre_re(root->rChild); //遍历右子树
    }
}

void traverse_In_re(binNode *root) {
    if (root == NULL) {
        return;
    } else {
        traverse_In_re(root->lChild); //遍历左子树
        visit(root);
        traverse_In_re(root->rChild); //遍历右子树
    }
}

void traverse_Pos_re(binNode *root) {
    if (root == NULL) {
        return;
    } else {
        traverse_Pos_re(root->lChild); //遍历左子树
        traverse_Pos_re(root->rChild); //遍历右子树
        visit(root);
    }
}


void printTree(binNode *root) {
    int dep = depth(root); //树的深度
    printf("\nDepth:%d\n", dep);
    binNode *level[1000] = {NULL,}; //层次遍历的数组
    int curr = 0; //队头
    int tail = 0; //队尾
    level[0] = root; //根节点入队
    while (curr <= pow(2, dep) - 1) { //看作一个满二叉树进行遍历
        if (root != NULL) {
            level[++tail] = root->lChild; //左子树入队
            level[++tail] = root->rChild; //右子树入队
        } else { //空节点就入队两个空孩子
            level[++tail] = NULL;
            level[++tail] = NULL;
        }
        root = level[++curr]; //下移一位
    }

    int i, j, k;
    int endofLine[10]; //第i层应该输出的节点数量
    int startPosofLine[10]; //每一行应该开始输出的坐标
    int margin[10]; //每一行输出坐标的间隔
    for (i = 0; i < 10; i++) { //计算输出的位置
        endofLine[i] = (int) pow(2, i);
        startPosofLine[i] = (int) pow(2, dep - i - 1) - 1;
        margin[i] = (int) pow(2, dep - i) - 1;
    }
    int count = 0; //计数输出的个数
    for (i = 0; i < dep; i++) { //输出每行
        for (j = 0; j < startPosofLine[i]; j++) printf("   "); //光标移动到起始位置
        for (j = 0; j < endofLine[i]; j++) {
            if (level[count] == NULL) { //如果是空节点，输出空格
                printf("   ");
                for (k = 0; k < margin[i]; k++) printf("   "); //输出间隔
                count++;
                continue;
            } else { //如果不是空节点，输出数据
                printf("%3c", level[count]->data);
                for (k = 0; k < margin[i]; k++) printf("   "); //输出间隔
                count++;
            }
        }
        printf("\n\n");
    }


}

bool isSym(binNode *root1, binNode *root2) {
    if (root1 == NULL && root2 == NULL) return true;//平凡情况:都是空节点
    if (root1 == NULL || root2 == NULL) return false;//平凡情况：只有一个为空
    if (root1->data != root2->data) return false; //平凡情况：数据不相等
    if (root1->data == root2->data)
        return (isSym(root1->rChild, root2->lChild) && isSym(root1->lChild, root2->rChild));
    return true;//THE COMPILER WARNED, SHUT IT UP
}

int main() {
    //中序后序序列
    char str[2 * MAX_SIZE];
    char preTravelOrder[MAX_SIZE];
    char inTravelOrder[MAX_SIZE];
    scanf("%s", str);
    //切割序列
    if(strchr(str,',')==NULL){
        printf("Invalid Sequence! Make sure to enter \‘,\’ in English Mode!\n"); //检查序列合法性
        exit(-1);
    }
    strcpy(preTravelOrder, strtok(str, ","));
    strcpy(inTravelOrder, strtok(NULL, ","));
    //确认相关信息
    int length = strlen(preTravelOrder);
    printf("SeqLength:%d\nPre:%s\nIn:%s\n", length, preTravelOrder, inTravelOrder);
    //重建二叉树
    binNode *newTree = rebuildTree(preTravelOrder, inTravelOrder);
    //输出后序序列
    printf("Post Order Travel: ");
    traverse_Pos_re(newTree);
    printf("\n");
    //输出树形
    printTree(newTree);
    //判断是否对称
    printf("\n");
    if (isSym(newTree->lChild, newTree->rChild))
        printf("This is Symmetry.\n");
    else
        printf("This is NOT Symmetry.\n");
    return 0;
}
