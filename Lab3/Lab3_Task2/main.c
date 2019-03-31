#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define isOp(x) ((x)=='+'||(x)=='-'||(x)=='*'||(x)=='/'||(x)=='('||(x)==')'||(x)=='\0') //�궨���ж��Ƿ�Ϊ�����
//��ֹ��ҲҪ����������������᲻��ѭ����ȥ
#define isLeaf(x) ( (x)->lChild == NULL && (x)->rChild == NULL ) //�궨���ж��Ƿ�ΪҶ�ڵ�
//�������ṹ������ֻ�����ֲŻᴢ����Ҷ�ڵ���
#define MAX_SIZE 1000
#define NUM_OPTR 7

int max(int a, int b) { return (a > b ? a : b); }

typedef enum _op {
    ADD, SUB, MUL, DIV, L_P, R_P, EOE
} Optr; //���������

Optr char2optr(char a) { //�������ת��Ϊ��Ӧ���������
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
        default: //�쳣����
            printf("Invalid Expression!\n");
            exit(-1);
    }
}

const char prior[NUM_OPTR][NUM_OPTR] = { //��������ȼ������ж�[ջ��][Ŀǰ]
        //��ǰ��ָ����� (�ᣩ��ջ����ָΪ��
        /*             +    -    *    /   (     )    \0   */
        /* + */       '>', '>', '<', '<', '<', '>', '>',
        /* - */       '>', '>', '<', '<', '<', '>', '>',
        /* * */       '>', '>', '>', '>', '<', '>', '>',
        /* / */       '>', '>', '>', '>', '<', '>', '>',
        /* ( */       '<', '<', '<', '<', '<', '=', ' ',
        /* ) */       ' ', ' ', ' ', ' ', ' ', ' ', ' ', //�����Ų�������ջ��
        /* \0 */      '<', '<', '<', '<', '<', ' ', '=',
};

typedef union _data { //���������壬����������ڵ����ݸ�ֵ
    double number;
    char op;
} Data;
typedef struct bT { //�������ڵ�����
    Data data; //������
    struct bT *lChild; //����ָ��
    struct bT *rChild; //����ָ��
} binNode;
typedef struct _stack { //��Ų��������������ջ
    int top; //ջ��λ��
    binNode *bNode[MAX_SIZE];
} Stack;

Stack *initStack() { //��ʼ��ջ
    Stack *ret = (Stack *) malloc(sizeof(Stack));
    if (ret == NULL) exit(-1);
    ret->top = -1;
    return ret;
}

bool isEmpty(Stack *S) { //�ж�ջ�Ƿ�Ϊ��
    if (S->top == -1) return true;
    return false;
}

void push(binNode *a, Stack *S) { //�����ڵ�a����ջS
    S->top++;
    S->bNode[S->top] = a;
}

binNode *pop(Stack *S) { //��ջ��
    if (isEmpty(S)) {
        printf("Invalid Expression!\n");
        exit(-1);
    } //��ֹ���
    S->top--;
    return S->bNode[S->top + 1];
}

binNode *top(Stack *S) { //����ջ��������
    if (isEmpty(S)) {
        printf("Invalid Expression!\n");
        exit(-1);
    } //��ֹ���
    return S->bNode[S->top];
}

double readDigit(char **c) { //��ȡ�������ĺ����������ƶ��ַ�ָ�룩
    //ע�⣺������������ַ�ָ�������ƶ��������мǲ��ܴ����ַ�������׵�ַ�����򽫻ᶪʧ�������ݣ�Ӧ�ô����ַ�ָ��ĸ���!!!
    double ret = 0.0;
    bool isFloat = false; //����С����ı��
    double carry = 10.0; //����С������ָ��
    do {
        if (**c == '.') {
            isFloat = true; //��С������
            *c += sizeof(char); //�ַ�ָ��ǰ��һλ
        }
        if (!isFloat) {
            ret *= 10; //��������һλ
            ret += **c - 48; //�Ѹ�λ������
            *c += sizeof(char); //�ַ�ָ��ǰ��һλ
            //����ָ����������⣿���*c++,�ӵ���sizeof(BT)������ʱ��BT�ǣ�����
        } else {
            ret += (**c - 48) * 1.0 / carry; //����С������
            *c += sizeof(char); //�ַ�ָ��ǰ��һλ
            carry *= 10.0; //ָ��λ����
        }
    } while (!isOp(**c)); //��������һ�������֮ǰ
    return ret; //���صõ��Ĳ�����
}


binNode *initbinNode(Data D) { //��ʼ���ڵ㣬ע�⣺����ֻ������Ҷ�ӽ�㣬���Բ���ȥ�ж�Union�������������
    binNode *ret = (binNode *) malloc(sizeof(binNode)); //����ռ�
    if (ret == NULL) exit(-1); //��ֹ���
    ret->data = D; //������ֵ
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

int depth(binNode *root) { //�������ĸ߶�
    if (root == NULL) return 0; //�����߶�Ϊ0
    else return (max(depth(root->lChild), depth(root->rChild)) + 1); //�ݹ鶨�壺���ĸ߶�Ϊ�����������߶�+1
}


void visit(binNode *bn) {
    if (isLeaf(bn)) printf("%g ", bn->data.number);
    else printf("%c ", bn->data.op);
}

void traverse_Pre_re(binNode *root) { //�������������
    if (root == NULL) {
        return;
    } else {
        visit(root);
        traverse_Pre_re(root->lChild);
        traverse_Pre_re(root->rChild);
    }
}

void traverse_In_re(binNode *root) { //�������������
    if (root == NULL) {
        return;
    } else {
        traverse_In_re(root->lChild);
        visit(root);
        traverse_In_re(root->rChild);
    }
}

void traverse_Pos_re(binNode *root) { //�������������
    if (root == NULL) {
        return;
    } else {
        traverse_Pos_re(root->lChild);
        traverse_Pos_re(root->rChild);
        visit(root);
    }
}

binNode *initbinNode_num(double a) { //�����ֳ�ʼ���������ڵ�
    Data d;
    d.number = a;
    return initbinNode(d);
}

binNode *initbinNode_op(char a) { //���������ʼ���������ڵ�
    Data d;
    d.op = a;
    return initbinNode(d);
}

double calculate(binNode *root) { //������ʽ����ֵ
    if (isLeaf(root)) return root->data.number; //�����Ҷ�ڵ㣬û�и��ţ��ͷ��ر����ֵ
    else {
        switch (root->data.op) { //�ݹ���������������ʽ��ֵ
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

void printTree(binNode *root) { //������ȫ���������������
    int dep = depth(root); //�������
    printf("\nDepth:%d\n", dep);
    binNode *level[1000] = {NULL,}; //��α���������
    int curr = 0;
    int tail = 0;
    level[0] = root; //���ڵ����
    while (curr <= pow(2, dep) - 1) {
        if (root != NULL) {
            //���Һ������
            level[++tail] = root->lChild;
            level[++tail] = root->rChild;
        } else {
            //�սڵ�պ������
            level[++tail] = NULL;
            level[++tail] = NULL;
        }
        root = level[++curr];
    }
    //printf("Number of Nodes:%d\n", curr);
    int i, j, k;
    int endofLine[10]; //��i��Ӧ������Ľڵ�����
    int startPosofLine[10]; //��i����ʼ���λ��
    int margin[10]; //��i��������
    for (i = 0; i < 10; i++) {
        endofLine[i] = (int) pow(2, i);
        startPosofLine[i] = (int) pow(2, dep - i - 1) - 1;
        margin[i] = (int) pow(2, dep - i) - 1;
    }

    int count = 0;
    for (i = 0; i < dep; i++) {
        for (j = 0; j < startPosofLine[i]; j++) printf("   "); //����ƶ�����ʼλ��
        for (j = 0; j < endofLine[i]; j++) {
            if (level[count] == NULL) { //�սڵ�����ո�
                printf("   ");
                for (k = 0; k < margin[i]; k++) printf("   "); //�ƶ����
                count++;
                continue;
            }
            if (!isLeaf(level[count])) { //Ҷ�ڵ��������
                printf("%3c", level[count]->data.op);
            } else { //�����ڵ���������
                printf("%3g", level[count]->data.number);
            }
            for (k = 0; k < margin[i]; k++) printf("   "); //������
            count++;
        }
        printf("\n\n");
    }


}


int main() {
    char str[100];
    //������ʽ
    printf("Input the expression:");
    scanf("%s", str);
    // printf("%s\n",str);
    Stack *optr = initStack();
    Stack *expTree = initStack();

    //���㷨���
    push(initbinNode_op('\0'), optr);//Ԥ����ջ
    char *c = &str[0];
    size_t t;
    size_t exp_len = strlen(str); //ȡ���ʽ����
    //�ڱ��ʽ���߼������ţ����㸺�ŵĴ���
    for (t = exp_len; t > 0; t--) {
        str[t] = str[t - 1];
    }
    str[0] = '(';
    str[exp_len + 1] = ')';
    str[exp_len + 2] = '\0'; //ĩβ���/0����ֹ��������
    bool isNegative = false; //�����Ĵ���
    do { //TODO:ѭ���˳�����
        binNode *curr;
        if (isOp(*c)) {
            //���ȼ��ж�
            int k = 0;
            char *tmp = c;
            while (*(tmp + 1) == '-') { //�ƶ�ָ�룬������������Ƿ���Ÿ���
                k++;
                tmp++;
            }
            if (k % 2 == 1) {
                isNegative = true;
            } //���������Ÿ���Ϊ����������һ������Ϊ��
            int top_op = char2optr(top(optr)->data.op);
            int cur_op = char2optr(*c);
            char priority = prior[top_op][cur_op]; //ջ�������|��ǰ�����
            switch (priority) {
                case '<'://��ǰ�����������ջ�������
                    push(initbinNode_op(*c), optr);//���������װ��һ���ڵ㣬���������ջ�У��ȴ��������ʽ��
                    c++; //ָ��ǰ��һλ��������ȡ
                    break;
                case '>': //ջ������������ڵ�ǰ����������Խ������ʽ����
                    curr = pop(optr); //�������ջ�е���һ�������
                    //�ӱ��ʽջ�е����������ʽ
                    binNode *crChild = pop(expTree);
                    binNode *clChild = pop(expTree);
                    //�����µ�����
                    curr->rChild = crChild;
                    curr->lChild = clChild;
                    //���µ�����ѹ����ʽջ
                    push(curr, expTree);
                    break;
                case '=': //���߶������ţ���ƥ�䣩
                    pop(optr); //����ջ��������
                    c++; //ָ��ǰ��һλ
                    break;
                default: //�쳣����
                    printf("Invalid Expression!\n");
                    exit(-1);

            }
            c = c + k;

        } else if (!isOp(*c) ){ //��������ѹջ����
            if (isNegative) {
                push(initbinNode_num(-(readDigit(&c))), expTree); //�Ǹ���
                isNegative = false; //���ñ�ǩ
            } else {
                push(initbinNode_num(readDigit(&c)), expTree); //���Ǹ���
            }
        } else {
            printf("Invalid Expression!\n");
            exit(-1);
        }
    } while (!isEmpty(optr));

    binNode *expression = pop(expTree); //��ջ�е��������ı��ʽ��
    printf("Prefix Expression: ");
    traverse_Pre_re(expression);
    printf("\n");
    //�ָ�ԭ���ı��ʽ��ȥ�����˵�����
    int i;
    for (i = 0; i < 98; i++) {
        str[i] = str[i + 1];
    }
    str[exp_len] = '\0';
    printf("Postfix Expression: ");
    traverse_Pos_re(expression);
    //������ʽ
    printf("\n%s = %g\n", str, calculate(expression));
    printf("Expression Tree:\n");
    //������ʽ����
    printTree(expression);
    return 0;
}
