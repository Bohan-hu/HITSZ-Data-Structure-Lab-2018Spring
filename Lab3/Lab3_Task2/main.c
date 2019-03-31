#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define isOp(x) ((x)=='+'||(x)=='-'||(x)=='*'||(x)=='/'||(x)=='('||(x)==')'||(x)=='\0') //宏定义判断是否为运算符
//终止符也要算作操作符，否则会不断循环下去
#define isLeaf(x) ( (x)->lChild == NULL && (x)->rChild == NULL ) //宏定义判断是否为叶节点
//二叉树结构解析：只有数字才会储存在叶节点上
#define MAX_SIZE 1000
#define NUM_OPTR 7

int max(int a, int b) { return (a > b ? a : b); }

typedef enum _op {
    ADD, SUB, MUL, DIV, L_P, R_P, EOE
} Optr; //运算符类型

Optr char2optr(char a) { //将运算符转换为对应的整数标号
    switch (a) {
        case '+':
            return ADD;
        case '-':
            return SUB;
        case '*':
            return MUL;
        case '/':
            return DIV;
        case '(':
            return L_P;
        case ')':
            return R_P;
        case '\0':
            return EOE;
        default: //异常处理
            printf("Invalid Expression!\n");
            exit(-1);
    }
}

const char prior[NUM_OPTR][NUM_OPTR] = { //运算符优先级次序判断[栈顶][目前]
        //当前所指运算符 (横），栈顶所指为列
        /*             +    -    *    /   (     )    \0   */
        /* + */       '>', '>', '<', '<', '<', '>', '>',
        /* - */       '>', '>', '<', '<', '<', '>', '>',
        /* * */       '>', '>', '>', '>', '<', '>', '>',
        /* / */       '>', '>', '>', '>', '<', '>', '>',
        /* ( */       '<', '<', '<', '<', '<', '=', ' ',
        /* ) */       ' ', ' ', ' ', ' ', ' ', ' ', ' ', //右括号不可能在栈顶
        /* \0 */      '<', '<', '<', '<', '<', ' ', '=',
};

typedef union _data { //数据联合体，方便二叉树节点数据赋值
    double number;
    char op;
} Data;
typedef struct bT { //二叉树节点类型
    Data data; //数据域
    struct bT *lChild; //左子指针
    struct bT *rChild; //右子指针
} binNode;
typedef struct _stack { //存放操作数或操作符的栈
    int top; //栈顶位置
    binNode *bNode[MAX_SIZE];
} Stack;

Stack *initStack() { //初始化栈
    Stack *ret = (Stack *) malloc(sizeof(Stack));
    if (ret == NULL) exit(-1);
    ret->top = -1;
    return ret;
}

bool isEmpty(Stack *S) { //判断栈是否为空
    if (S->top == -1) return true;
    return false;
}

void push(binNode *a, Stack *S) { //将树节点a推入栈S
    S->top++;
    S->bNode[S->top] = a;
}

binNode *pop(Stack *S) { //将栈顶
    if (isEmpty(S)) {
        printf("Invalid Expression!\n");
        exit(-1);
    } //防止抽风
    S->top--;
    return S->bNode[S->top + 1];
}

binNode *top(Stack *S) { //返回栈顶操作符
    if (isEmpty(S)) {
        printf("Invalid Expression!\n");
        exit(-1);
    } //防止抽风
    return S->bNode[S->top];
}

double readDigit(char **c) { //读取操作数的函数（将会移动字符指针）
    //注意：这个函数对于字符指针做了移动，所以切记不能传入字符数组的首地址，否则将会丢失整个数据，应该传入字符指针的副本!!!
    double ret = 0.0;
    bool isFloat = false; //碰到小数点的标记
    double carry = 10.0; //碰到小数点后的指数
    do {
        if (**c == '.') {
            isFloat = true; //有小数部分
            *c += sizeof(char); //字符指针前移一位
        }
        if (!isFloat) {
            ret *= 10; //数字左移一位
            ret += **c - 48; //把个位加上来
            *c += sizeof(char); //字符指针前移一位
            //二级指针的运算问题？如果*c++,加的是sizeof(BT)，而此时的BT是？？？
        } else {
            ret += (**c - 48) * 1.0 / carry; //加上小数部分
            *c += sizeof(char); //字符指针前移一位
            carry *= 10.0; //指数位左移
        }
    } while (!isOp(**c)); //在碰到下一个运算符之前
    return ret; //返回得到的操作数
}


binNode *initbinNode(Data D) { //初始化节点，注意：数字只存在于叶子结点，所以不用去判断Union里面的数据类型
    binNode *ret = (binNode *) malloc(sizeof(binNode)); //分配空间
    if (ret == NULL) exit(-1); //防止抽风
    ret->data = D; //数据域赋值
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

int depth(binNode *root) { //二叉树的高度
    if (root == NULL) return 0; //空树高度为0
    else return (max(depth(root->lChild), depth(root->rChild)) + 1); //递归定义：树的高度为左右子树最大高度+1
}


void visit(binNode *bn) {
    if (isLeaf(bn)) printf("%g ", bn->data.number);
    else printf("%c ", bn->data.op);
}

void traverse_Pre_re(binNode *root) { //先序遍历二叉树
    if (root == NULL) {
        return;
    } else {
        visit(root);
        traverse_Pre_re(root->lChild);
        traverse_Pre_re(root->rChild);
    }
}

void traverse_In_re(binNode *root) { //中序遍历二叉树
    if (root == NULL) {
        return;
    } else {
        traverse_In_re(root->lChild);
        visit(root);
        traverse_In_re(root->rChild);
    }
}

void traverse_Pos_re(binNode *root) { //后序遍历二叉树
    if (root == NULL) {
        return;
    } else {
        traverse_Pos_re(root->lChild);
        traverse_Pos_re(root->rChild);
        visit(root);
    }
}

binNode *initbinNode_num(double a) { //以数字初始化二叉树节点
    Data d;
    d.number = a;
    return initbinNode(d);
}

binNode *initbinNode_op(char a) { //以运算符初始化二叉树节点
    Data d;
    d.op = a;
    return initbinNode(d);
}

double calculate(binNode *root) { //计算表达式树的值
    if (isLeaf(root)) return root->data.number; //如果是叶节点，没有负号，就返回本身的值
    else {
        switch (root->data.op) { //递归计算左右子树表达式的值
            case '+':
                return calculate(root->lChild) + calculate(root->rChild);
            case '*':
                return calculate(root->lChild) * calculate(root->rChild);
            case '/':
                return calculate(root->lChild) / calculate(root->rChild);
            case '-':
                return calculate(root->lChild) - calculate(root->rChild);
            default:
                printf("Invalid Expression!\n");
                exit(-1);
        }
    }
}

void printTree(binNode *root) { //将树补全成满二叉树后输出
    int dep = depth(root); //树的深度
    printf("\nDepth:%d\n", dep);
    binNode *level[1000] = {NULL,}; //层次遍历的数组
    int curr = 0;
    int tail = 0;
    level[0] = root; //根节点入队
    while (curr <= pow(2, dep) - 1) {
        if (root != NULL) {
            //左右孩子入队
            level[++tail] = root->lChild;
            level[++tail] = root->rChild;
        } else {
            //空节点空孩子入队
            level[++tail] = NULL;
            level[++tail] = NULL;
        }
        root = level[++curr];
    }
    //printf("Number of Nodes:%d\n", curr);
    int i, j, k;
    int endofLine[10]; //第i层应该输出的节点数量
    int startPosofLine[10]; //第i层起始输出位置
    int margin[10]; //第i层输出间隔
    for (i = 0; i < 10; i++) {
        endofLine[i] = (int) pow(2, i);
        startPosofLine[i] = (int) pow(2, dep - i - 1) - 1;
        margin[i] = (int) pow(2, dep - i) - 1;
    }

    int count = 0;
    for (i = 0; i < dep; i++) {
        for (j = 0; j < startPosofLine[i]; j++) printf("   "); //光标移动到起始位置
        for (j = 0; j < endofLine[i]; j++) {
            if (level[count] == NULL) { //空节点输出空格
                printf("   ");
                for (k = 0; k < margin[i]; k++) printf("   "); //移动光标
                count++;
                continue;
            }
            if (!isLeaf(level[count])) { //叶节点输出数字
                printf("%3c", level[count]->data.op);
            } else { //其他节点输出运算符
                printf("%3g", level[count]->data.number);
            }
            for (k = 0; k < margin[i]; k++) printf("   "); //输出间隔
            count++;
        }
        printf("\n\n");
    }


}


int main() {
    char str[100];
    //读入表达式
    printf("Input the expression:");
    scanf("%s", str);
    // printf("%s\n",str);
    Stack *optr = initStack();
    Stack *expTree = initStack();

    //主算法框架
    push(initbinNode_op('\0'), optr);//预处理栈
    char *c = &str[0];
    size_t t;
    size_t exp_len = strlen(str); //取表达式长度
    //在表达式两边加上括号，方便负号的处理
    for (t = exp_len; t > 0; t--) {
        str[t] = str[t - 1];
    }
    str[0] = '(';
    str[exp_len + 1] = ')';
    str[exp_len + 2] = '\0'; //末尾添加/0，防止垃圾数据
    bool isNegative = false; //负数的处理
    do { //TODO:循环退出条件
        binNode *curr;
        if (isOp(*c)) {
            //优先级判断
            int k = 0;
            char *tmp = c;
            while (*(tmp + 1) == '-') { //移动指针，看运算符后面是否跟着负号
                k++;
                tmp++;
            }
            if (k % 2 == 1) {
                isNegative = true;
            } //若紧跟负号个数为奇数，则下一个数字为负
            int top_op = char2optr(top(optr)->data.op);
            int cur_op = char2optr(*c);
            char priority = prior[top_op][cur_op]; //栈顶运算符|当前运算符
            switch (priority) {
                case '<'://当前运算符优先于栈顶运算符
                    push(initbinNode_op(*c), optr);//把运算符封装成一个节点，推入运算符栈中，等待创建表达式树
                    c++; //指针前进一位，继续读取
                    break;
                case '>': //栈顶运算符优先于当前运算符，可以建立表达式子树
                    curr = pop(optr); //从运算符栈中弹出一个运算符
                    //从表达式栈中弹出两个表达式
                    binNode *crChild = pop(expTree);
                    binNode *clChild = pop(expTree);
                    //建立新的子树
                    curr->rChild = crChild;
                    curr->lChild = clChild;
                    //将新的子树压入表达式栈
                    push(curr, expTree);
                    break;
                case '=': //两者都是括号（且匹配）
                    pop(optr); //弹出栈顶的括号
                    c++; //指针前进一位
                    break;
                default: //异常处理
                    printf("Invalid Expression!\n");
                    exit(-1);

            }
            c = c + k;

        } else if (!isOp(*c) ){ //读入数字压栈部分
            if (isNegative) {
                push(initbinNode_num(-(readDigit(&c))), expTree); //是负数
                isNegative = false; //重置标签
            } else {
                push(initbinNode_num(readDigit(&c)), expTree); //不是负数
            }
        } else {
            printf("Invalid Expression!\n");
            exit(-1);
        }
    } while (!isEmpty(optr));

    binNode *expression = pop(expTree); //从栈中弹出完整的表达式树
    printf("Prefix Expression: ");
    traverse_Pre_re(expression);
    printf("\n");
    //恢复原来的表达式，去掉两端的括号
    int i;
    for (i = 0; i < 98; i++) {
        str[i] = str[i + 1];
    }
    str[exp_len] = '\0';
    printf("Postfix Expression: ");
    traverse_Pos_re(expression);
    //计算表达式
    printf("\n%s = %g\n", str, calculate(expression));
    printf("Expression Tree:\n");
    //输出表达式的树
    printTree(expression);
    return 0;
}
