#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> // time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
    int         data;
    struct node* left;
    struct node* right;
} NODE;

typedef struct
{
    NODE* root;     //트리의 루트노드를 가리키는 헤드 구조 (루트노드의 포인터를 갖고있는 헤드 스트럭쳐)
} TREE;

////////////////////////////////////////////////////////////////////////////////

TREE* BST_Create(void) {
    TREE* tree = (TREE*)malloc(sizeof(TREE));
    tree->root = NULL;
    return tree;
}






NODE* _makeNode(int data) {
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}





static void _insert(NODE* root, NODE* newPtr) {
   
}


int BST_Insert(TREE* pTree, int data) {
    NODE* newPtr = _makeNode(data);
    if (newPtr) {
        if (!pTree->root)
            pTree->root = newPtr;
        else {
            NODE* target = pTree->root;
            while (1) {
                if (target->data <= data) {
                    if (!target->right) {
                        target->right = newPtr;
                        break;
                    }
                    else {
                        target = target->right;
                    }
                }
                else {
                    if (!target->left) {
                        target->left = newPtr;
                        break;
                    }
                    else {
                        target = target->left;
                    }
                }

            }
        }
        return 1;
    }
    else {
        return 0;
    }
}




static NODE* _delete(NODE* root, int dltKey, int* success) {

   if(!root){
        *success = 0;
        return NULL;
   }

   if(dltKey < root->data){
    root->left =  _delete(root->left,dltKey,success);
   }
   else if(dltKey > root->data){
    root->right = _delete(root->right,dltKey,success);
   }
   else{
        NODE *newroot;
        if(root->left == NULL){ //왼쪽자식 X
            newroot = root->right;
            free(root);
            *success = 1;
            return newroot;
        }
        else if(root->right ==NULL){    //오른쪽자식 X
            newroot = root->left;
            free(root);
            *success = 1;
            return newroot;
        }
        else{   //둘다 자식있음
            newroot = root->right;
            while(newroot->left){
                newroot = newroot->left;
            }
            root->data = newroot->data;
            newroot->data = dltKey;
            root->right = _delete(root->right, dltKey, success);
        }
    }
    return root;
}

static void _destroy(NODE* root);




int BST_Delete(TREE* pTree, int dltKey) {
    int success;
    NODE *new = _delete(pTree->root, dltKey, &success);

    if(success){
        pTree->root = new;
    }

    return success;
}







static void _traverse(NODE* root) {
    if (root) {
        if (root->left) {
            _traverse(root->left);
        }
        printf("%d ", root->data);
        _traverse(root->right);
    }
}
void BST_Traverse(TREE* pTree) {
    _traverse(pTree->root);
}



static void _infix_print(NODE root, int level) {
    if (root.right) {
        _infix_print(*(root.right), level + 1);
    }
    for (int i = 0; i < level; i++) {
        printf("\t");
    }
    printf("%d\n", root.data);
    if (root.left) {
        _infix_print(*(root.left), level + 1);
    }
}

void printTree(TREE* pTree) {
    _infix_print(*(pTree->root), 0); 

    return;
}




int BST_Empty(TREE* pTree){
    if(pTree->root == NULL){
        return 1;
    }
    else
        return 0;
}



static void _destroy(NODE* root){
    if (root) {   
        _destroy(root->left);
        _destroy(root->right);
        free(root);
    }

};








/* Deletes all data in tree and recycles memory
*/
//트리를 메모리에서 완전히 해지  :  노드들도 따라다니면서 모두 제거하고 헤드도 제거해주기
void BST_Destroy(TREE* pTree){
    if (pTree) {
      _destroy(pTree->root);
      free(pTree);
   }
};
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    TREE* tree;
    int data;

    // creates a null tree
    tree = BST_Create();

    if (!tree)
    {
        printf( "Cannot create a tree!\n");
        return 100;
    }

    fprintf(stdout, "How many numbers will you insert into a BST: ");

    int numbers;
    scanf("%d", &numbers);

    fprintf(stdout, "Inserting: ");

    srand(time(NULL));
    for (int i = 0; i < numbers; i++)
    {
        data = rand() % (numbers * 3) + 1; // random number (1 ~ numbers * 3) 

        fprintf(stdout, "%d ", data);

        // insert funtion call
        int ret = BST_Insert( tree, data);
        if (!ret) break;
    }
    fprintf(stdout, "\n");

    // inorder traversal
    fprintf(stdout, "Inorder traversal: ");
    BST_Traverse( tree);
    fprintf(stdout, "\n");

    // print tree with right-to-left infix traversal
    fprintf(stdout, "Tree representation:\n");
    printTree(tree);





    while (1)
    {
        fprintf(stdout, "Input a number to delete: ");
        int num;
        int ret = scanf("%d", &num);
        if (ret != 1) break;

        ret = BST_Delete( tree, num);
        if (!ret)
        {
            fprintf( stdout, "%d not found\n", num);
            continue;
        }

        if (BST_Empty( tree))
        {
            fprintf( stdout, "Empty tree!\n");
            break;
        }
        
        // print tree with right-to-left infix traversal
        fprintf(stdout, "Tree representation:\n");
        printTree(tree);

        
    }

    BST_Destroy( tree);

    return 0;
}