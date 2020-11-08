
#include <stdlib.h> 
#include <stdio.h>
#include <string.h> 
#include <ctype.h> 


#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define SEARCH			4
#define DELETE			5
#define COUNT			6


typedef struct 
{
	char	*token; 
	int		freq;
} tTOKEN; 

typedef struct node
{
	tTOKEN		*dataPtr;
	struct node	*llink;
	struct node	*rlink;
} NODE;

typedef struct
{
	int		count;
	NODE	*pos; 
	NODE	*head;
	NODE	*rear;
} LIST;

static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu);
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr);
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr);
tTOKEN *createToken( char *str);
void destroyToken( tTOKEN *pToken);

LIST *createList( void){
	LIST * list = (LIST*)malloc(sizeof(LIST));
	list->head = NULL;
	list->rear = NULL;
	list->count = 0;
	return list; 
};

void destroyList( LIST *pList){
	NODE* ppre ;
	NODE* ploc = pList->head;
	while(ploc != NULL)
	{
		destroyToken(ploc->dataPtr);
		ppre = ploc;
		ploc = (ploc->rlink);
		free(ppre);
	}
		free(pList);
};


int addNode( LIST *pList, tTOKEN *dataInPtr){
	NODE* pPre = NULL;
	int insert_result = _insert( pList, pPre, dataInPtr);

	if(insert_result == 0) return 0;
	else if(insert_result == 1) return 1;
	else return 2;
};

int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut){
	NODE* ppre = NULL;
	NODE* ploc = NULL;
	keyPtr[0] = toupper(keyPtr[0]);
	for (int i = 1; i < strlen(keyPtr); ++i)
	{
		keyPtr[i] = tolower(keyPtr[i]);
	}
	if( _search( pList, &ppre, &ploc, keyPtr) ){ 
		_delete( pList, ppre, ploc,dataOut);
		return 1;
	}
	else return 0;
};

int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut){
	NODE* ppre=NULL;
	NODE* ploc=NULL;
	pArgu[0] = toupper(pArgu[0]);
	for (int i = 1; i < strlen(pArgu); ++i)
	{
		pArgu[i] = tolower(pArgu[i]);
	}
	if(_search( pList, &ppre, &ploc, pArgu) ){ 
		*pDataOut = (tTOKEN*)(ploc->dataPtr);  
		return 1;
	}else return 0;
};



int countList( LIST *pList){
	return pList->count ;
};


int emptyList( LIST *pList){
	if(pList->count==0) return 0;
	else return 1;
};



void printList( LIST *pList){
	NODE* ploc = pList->head; 
	while(ploc != NULL){
		printf("%s \t %d \n",ploc->dataPtr->token, (ploc->dataPtr->freq));
		ploc = ploc->rlink;
	}
};



void printListR( LIST *pList){
	NODE* ploc = pList->rear;
	while(ploc != NULL){
		printf("%s \t %d \n",ploc->dataPtr->token, (ploc->dataPtr->freq));
		ploc = ploc->llink;
	}
};
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr){
	NODE* newN = (NODE*)malloc(sizeof(NODE));
	if( !newN ) return 0;
	NODE* pLoc;
    int found = _search( pList, &pPre, &pLoc, dataInPtr->token);
  	if(found==0){ 
  		newN->llink = NULL;
  		newN->rlink = NULL;
    	newN->dataPtr = dataInPtr;  

     	if(pPre==NULL){ 
     		newN->rlink = pList->head;
     		pList->head = newN;
     		if(pList->count > 0){
     			pLoc->llink = newN;
     		}else pList->rear = newN;
     	}
      	else{ 
      		newN->rlink = pLoc;
      		newN->llink = pPre;
      		pPre->rlink = newN;
      		if(pLoc == NULL) pList->rear = newN;
      		else pLoc->llink = newN;
      	} 
      	pLoc = newN;
      	pList->count++;
      	return 1; 
   	}else 
   	{
   		free(newN);
   		pLoc->dataPtr->freq++;
   		return 2;
   	}
};

static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr){
	*dataOutPtr = pLoc->dataPtr;
	if(pLoc->llink !=NULL){
		pPre = pLoc->llink;
		pPre->rlink = pLoc->rlink; 
	}else pList->head = pLoc->rlink;
	if(pLoc->rlink !=NULL){
		pLoc->rlink->llink = pLoc->llink;
	}
	else pList->rear = pLoc->llink;
	pList->count--;
	free(pLoc);
}; 



static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu){
	*pPre = NULL;
	if( pList->head == NULL ) return 0;
	*pLoc = pList->head;

	while(*pLoc !=  NULL)
	{	
		if(strcmp(pArgu, (*pLoc)->dataPtr->token) == 0) return 1; 
		if(strcmp(pArgu, (*pLoc)->dataPtr->token) < 0 ) return 0;
		*pPre = *pLoc;
		*pLoc = (*pLoc)->rlink;
	}
	return 0;
};



tTOKEN *createToken( char *str){
	tTOKEN* newtoken = (tTOKEN*)malloc(sizeof(tTOKEN));
	newtoken->token = strdup(str); 
	newtoken->freq = 1;

	return newtoken;
};



void destroyToken( tTOKEN *pToken){
	free(pToken->token);
	free(pToken);
};



int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'F':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'S':
			return SEARCH;
		case 'D':
			return DELETE;
		case 'C':
			return COUNT;
	}
	return 0; 
}

int main( int argc, char **argv)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;
	FILE *fp;
	
	if (argc != 2){
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	

	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}

	while(fscanf( fp, "%s", str) == 1)
	{
		pToken = createToken(str);
		ret = addNode( list, pToken);  
		if (ret == 2) 
		destroyToken( pToken);
	}
	
	fclose( fp);
	
	fprintf( stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");
	
	while (1)
	{	
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				destroyList( list);
				return 0;
			
			case FORWARD_PRINT:
				printList( list);
				break;
			
			case BACKWARD_PRINT:
				printListR( list);
				break;
			
			case SEARCH:
				fprintf( stderr, "Input a string to find: ");
				fscanf( stdin, "%s", str);
				
				if (searchList( list, str, &pToken))
				{
					fprintf( stdout, "(%s, %d)\n", pToken->token, pToken->freq);
				}
				else fprintf( stdout, "%s not found\n", str);
				break;
				
			case DELETE:
				fprintf( stderr, "Input a string to delete: ");
				fscanf( stdin, "%s", str);
				
				if (removeNode( list, str, &pToken))
				{
					fprintf( stdout, "%s deleted\n", pToken->token);
					destroyToken( pToken);
				}
				else fprintf( stdout, "%s not found\n", str);
				break;
			
			case COUNT:
				fprintf( stdout, "%d\n", countList( list));
				break;
			}
		
		if (action) fprintf( stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}
