#include <stdlib.h> 
#include <stdio.h>
#include <ctype.h> 
#include <assert.h> 


#define MAX_STACK_SIZE   50 


typedef struct node
{
    char      data;
    struct node* left;
    struct node* right;
} NODE;

typedef struct
{
    NODE* root;
} TREE; 


TREE* createTree(void) {
    TREE* head = (TREE*)malloc(sizeof(TREE));
    head->root = NULL;
    return head;
}


void _destroy(NODE* root) {
    if (root) {
        if (root->right) {
            _destroy(root->right);
        }
        if (root->left) {
            _destroy(root->left);
        }
        free(root);
    }
}
void destroyTree(TREE* pTree)
{
    if (pTree)
    {
        _destroy(pTree->root);
    }
    free(pTree);
}


NODE* _makeNode(char ch) {
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->data = ch;
    node->left = NULL;
    node->right = NULL;
    return node;
}


void _traverse(NODE* root) {
    if (root->left) {
        printf("(");
        _traverse(root->left);
    }
    printf("%c", root->data);
    if (root->right) {
        _traverse(root->right);
        printf(")");
    }
}


int postfix2tree(char* expr, TREE* pTree) {
    NODE* stack[MAX_STACK_SIZE];

    int top = -1;
    int i = 0;
    while (1) {
        char ch = *(expr + i);
        if (ch == '\0') {
            break;
        }
        NODE* node = _makeNode(ch);
        switch (ch) {
            case '+':
            case '-':
            case '/':
            case '*':
                if (top == -1) {
                    _destroy(node);
                    return 0;
                }
                node->right = stack[top--];
                if (top == -1) {
                    _destroy(node);
                    return 0;
                }
                node->left = stack[top--];
                break;
            default:
                break;
        }
        stack[++top] = node;
        i++;
    }
    if (top == 0) {
        NODE* root = stack[top--];
        pTree->root = root;
        return 1;
    }
    else {
        for (i = top; i > -1; i--) {
            _destroy(stack[i]);
        }
        return 0;
    }
    return 1;
}

void traverseTree(TREE* pTree);
void printTree(TREE* pTree);

void _infix_print(NODE root, int level) {
    if (root.right) {
        _infix_print(*(root.right), level + 1);
    }
    for (int i = 0; i < level; i++) {
        printf("\t");
    }
    printf("%c\n", root.data);
    if (root.left) {
        _infix_print(*(root.left), level + 1);
    }
}


float evalPostfix(char* expr) {
    float stack[MAX_STACK_SIZE];

    int top = -1;
    int i = 0;
    while (1) {
        char ch = *(expr + i);
        if (ch == '\0') {
            break;
        }
        float right;
        float left;

        switch (ch) {
        case '+':
            right = stack[top--];
            left = stack[top--];
            stack[++top] = right + left;
            break;
        case '-':
            right = stack[top--];
            left = stack[top--];
            stack[++top] = right - left;
            break;
        case '/':
            right = stack[top--];
            left = stack[top--];
            stack[++top] = right / left;
            break;
        case '*':
            right = stack[top--];
            left = stack[top--];
            stack[++top] = right * left;
            break;
        default:
            stack[++top] = (float)(ch - '0');
            break;
        }
        i++;
    }
    float result = stack[top--];
    return result;
}


void printTree(TREE* pTree)
{
    _infix_print(*(pTree->root), 0); //처음에는 레벵 0에서 시작, 루트 위치가 레벨 0이고
    //루트의 자식은 레벨1, 그 자식들의 자식은 레벨2, 이 레벨에 따라 탭을 하나할지 2개할지
    //탭을 쓰지 않을지가 결정이됨

    return;
}


void traverseTree(TREE* pTree)
{
    _traverse(pTree->root);

    return;
}


int main(int argc, char** argv)
{
    TREE* tree;
    char expr[1024];

    fprintf(stdout, "\nInput an expression (postfix): "); //후위수식으로 표현을 입력하라


    while (fscanf(stdin, "%s", expr) == 1)
    {
        tree = createTree();

        if (!tree)
        {
            printf("Cannot create tree\n");
            return 100;
        }


        int ret = postfix2tree(expr, tree); 

        if (!ret)
        {
            fprintf(stdout, "invalid expression!\n");
            destroyTree(tree); 
         
            
            continue;
        }

     
        fprintf(stdout, "\nInfix expression : ");
        traverseTree(tree); 


        fprintf(stdout, "\n\nTree representation:\n");
        printTree(tree); 
        float val = evalPostfix(expr); 
        fprintf(stdout, "\nValue = %f\n", val); 

 
        destroyTree(tree); 

        fprintf(stdout, "\nInput an expression (postfix): ");
    }
    return 0;
}