#define SHOW_STEP 0
#define BALANCING 1

#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root){
	if(!root)
		return 0;
	return root->height;
}


/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root){
	NODE* tmp;
	tmp = root->left;
	root->left = tmp->right;
	tmp->right = root;

	int leftheigth = getHeight(root->left);
	int rightheigth = getHeight(root->right);

	if(leftheigth <= rightheigth)
		root->height = rightheigth + 1;
	else
		root->height = leftheigth + 1;

	leftheigth = getHeight(tmp->left);
	rightheigth = getHeight(tmp->right);

	if(leftheigth <= rightheigth)
		tmp->height = rightheigth + 1;
	else
		tmp->height = leftheigth + 1;

	return tmp;
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root){
	NODE* tmp;
	tmp = root->right;
	root->right = tmp->left;
	tmp->left = root;

	int leftheigth = getHeight(root->left);
	int rightheigth = getHeight(root->right);

	if(leftheigth <= rightheigth)
		root->height = rightheigth + 1;
	else
		root->height = leftheigth + 1;

	leftheigth = getHeight(tmp->left);
	rightheigth = getHeight(tmp->right);

	if(leftheigth <= rightheigth)
		tmp->height = rightheigth + 1;
	else
		tmp->height = leftheigth + 1;

	return tmp;
}


/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void){
	AVL_TREE * tree;
	tree = (AVL_TREE*)malloc(sizeof(AVL_TREE));
	if(tree){
		tree->root = NULL;
		tree->count = 0;
	}
	return tree;
}

/* Deletes all data in tree and recycles memory
*/
static void _destroy(NODE *root) {
   if (root == NULL)
      return;
   NODE* left = root->left;
   NODE* right = root->right;
	free(root->data); 
	free(root);
   _destroy(left);
   _destroy(right);
   
}
void AVL_Destroy(AVL_TREE *pTree) {
   _destroy(pTree->root);
   free(pTree);
}
/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr){

	NODE *tmp = root;
	if(!root){
		return newPtr;
	}

	if(strcmp(newPtr->data, root->data) >= 0){
		root->right = _insert(root->right,newPtr);
	}
	else{
		root->left = _insert(root->left,newPtr);
	}

#if BALANCING
	int leftheigth = getHeight(root->left);
	int rightheigth = getHeight(root->right);

	if(leftheigth <= rightheigth)
		root->height = rightheigth + 1;
	else
		root->height = leftheigth + 1;

	int val = leftheigth - rightheigth;
	if(val >= -1){
		if(val >1 ){
			if(getHeight(root->left->left) <= getHeight(root->left->right))
				root->left = rotateLeft(root->left);
			tmp = rotateRight(root);
		}
	}
	else{
		if(getHeight(root->right->right) <= getHeight(root->right->left))
			root->right = rotateRight(root->right);
		tmp = rotateLeft(root);
	}
#endif

	return tmp;
	
}

static NODE *_makeNode( char *data){
	NODE* node = (NODE*)malloc(sizeof(NODE));
	if(!node)
		return NULL;
    node->data = strdup(data);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

 
/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data){
	NODE* newPtr = _makeNode(data);
	if(!newPtr)
		return 0;
	pTree->root = _insert(pTree->root, newPtr);
	pTree->count++;
	return 1;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, char *key){
	NODE *tmp = root;
	while(tmp){
		if(strcmp(key,tmp->data) == 0){
			return tmp;
		}
		if(strcmp(key, tmp->data) > 0){
			tmp = tmp->right;
		}
		else{
			tmp = tmp->left;
		}
	}
	return tmp;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key){

	if(!pTree)
		return NULL;
	NODE *found = _retrieve(pTree->root, key);
	if(found)
		return found->data;
	else 
		return NULL;

}



/* Prints tree using inorder traversal
*/
static void _traverse( NODE *root){
	if (root) {
        if (root->right) {
            _traverse(root->right);
        }
        printf("%s ", root->data);
        _traverse(root->left);
    }
}
void AVL_Traverse( AVL_TREE *pTree){
    _traverse(pTree->root);
}



/* internal traversal function
*/
static void _infix_print( NODE *root, int level){
	if (root->right) {
        _infix_print((root->right), level + 1);
    }
    for (int i = 0; i < level; i++) {
        printf("\t");
    }
    printf("%s\n", root->data);
    if (root->left) {
        _infix_print((root->left), level + 1);
    }
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree){
	_infix_print((pTree->root), 0); 
    return;
}


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	
	// creates a null tree
	tree = AVL_Create();
	
	if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	} 

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		// insert function call
		AVL_Insert( tree, str);

#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		printTree( tree);
#endif
	}
	
	fclose( fp);
	
#if SHOW_STEP
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	// retrieval
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{



		key = AVL_Retrieve( tree, str);
		
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		
		fprintf( stdout, "Query: ");
	}
	
	//destroy tree

	AVL_Destroy( tree);

	return 0;
}