//이번에는 주석을 제출하지 않고 과제를 제출하였습니다. (여태까지 주석을 모두 제거하고 제출하는 것이 원칙인지 알았습니다.)
#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time
#define MAX_ELEM 20  //20개의 난수를 발생시킨다. 할때의 20 ---->이 값을 바꿔서 컴파일하면 바꾼 값만큼의 난수가 발생




//HEAP이라는 자료구조. HEAP의 headstructure라고 하면 됨. 
typedef struct
{
	int *heapArr;	//heapArr에 메모리를 할당받아서, 메모리를 할당받은 그 주소를 가르키는 포인터로 사용되는 heapArr

					//left child는 2i+1
					//right child는 2i+2


	int	last;		//last는 지금 heapArr에 원소가 어느 index까지 차있다는 것을 알려줌 ex) heap에 원소 3 하나만 들어가있느면 last는 0
					//last의 초기값은 -1로 주면된다
					//heap에 하나씩 집어넣을때마다 last가 1씩 커짐  

	int	capacity;	//배열의 용량 최대 몇까지 활용할 수 있다는 정보가 들어가있어야함.
					//상수 MAX_ELEM만큼 capacity를 지정
} HEAP;




/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
heapArr에도 capacity만큼 메모리 할당해주기

*/
HEAP *heapCreate(int capacity)
{
	HEAP* heaphead = (HEAP*)calloc(1, sizeof(HEAP));
	if (heaphead != NULL)
	{
		heaphead->capacity = capacity;
		heaphead->last = -1;
		heaphead->heapArr = (int*)calloc(capacity, sizeof(int));
		return heaphead;
	}
	else return NULL;
};





/* Free memory for heap
힙을 위해 할당해줬던 메모리를 전부 해제해주기
힙헤드 구조체까지 싹 다

*/
void heapDestroy(HEAP *heap)
{
	if (heap->heapArr == NULL)
	{
		free(heap);
	}
	else 
	{
		free(heap->heapArr);
		free(heap);
	}
};



/* Reestablishes heap by moving data in child up to correct location heap array
내부함수이다
heapinsert가 사용하는 함수이다.

이함수 실행하면서 heap의 last값을 증가시키면서 거기다가 일단 data를 넣어놓고 거기서 reheapup해서 적절한 자기위치를 찾아서 올라감

이 함수를 실행하면 지금 삽입하는 값을 포함해서 최고값이 index 0번째에 있음을 유지
*/
static void _reheapUp(HEAP *heap, int index)
{
	//if (index != 0)
	
		int* parent = heap->heapArr + (int)( (index - 1)/2 );
		if (heap->heapArr[index]  >  *parent)
		{
			int tmp = *parent;
			*parent = heap->heapArr[index];
			heap->heapArr[index] = tmp;
			int next_index = (int)((index - 1) / 2);

			_reheapUp(heap, next_index);

		}
	

};







/* Inserts data into heap
return 1 if successful; 0 if heap full
난수 발생된 숫자를 힙에 하나씩 집어넣는것
return 1 삽입 성공
return 0 힙이 이미 capacity만큼 꽉차있어서 더 삽입 못할때
*/
int heapInsert(HEAP *heap, int data)
{
	if ((heap->capacity - 1) == heap->last) return 0;
	else
	{
		heap->last++;					//먼저 heap last를 1 증가시켜주고
		heap->heapArr[heap->last] = data;	//그 heap last번째의 array칸에다가 데이터를 보관

		_reheapUp(heap, heap->last);			//heap last의 값을 리힙업 함수의 두번째 인수로 (방금 값을 1 올린)heap.last를 넣어준다 
		return 1;
	}
};








/* Reestablishes heap by moving data in root down to its correct location in the heap
index를 주긴했지만 삭제는 root만 삭제하기 때문에 두번째 argument인 index는 항상 0이다.
heapDelete가 사용하는 내부함수이다.
*/
static void _reheapDown(HEAP *heap, int index) 
{
	if (index * 2 + 1 == heap->last)
	{
		int * largest; 
		if( heap->heapArr[0] > heap->heapArr[index * 2 + 1] )
			largest = heap->heapArr;
		else largest = heap->heapArr + index * 2 + 1;
		int tmp = heap->heapArr[0];
		heap->heapArr[0] = *largest;
		*largest = tmp;



	}
	else
	{

		int leftkey = heap->heapArr[index * 2 + 1];
		int* rightkey = NULL;

		if (index * 2 + 2 <= heap->last)
		{
			rightkey = heap->heapArr + index * 2 + 2;

			int flag = 0;
			int* largesubtree = NULL;
			if (leftkey > *rightkey)
			{
				flag = 1;
				largesubtree = heap->heapArr + index * 2 + 1;

			}
			else largesubtree = heap->heapArr + index * 2 + 2;
			if (heap->heapArr[index] < *largesubtree)
			{
				int tmp = heap->heapArr[index];
				heap->heapArr[index] = *largesubtree;
				*largesubtree = tmp;


				if (flag == 1) {
					flag = 0;
					_reheapDown(heap, index * 2 + 1);
				}
				else _reheapDown(heap, index * 2 + 2);

			}
		}



	}
	
};


/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
root에서 하나의 원소의 값을 끄집어 내어서 이 data에다가 저장해줌
그를 위해서는 reheapDown을 호출한다
*/
int heapDelete(HEAP *heap, int* data) {
	if (heap->heapArr == NULL) return 0;
	else
	{
		*data = heap->heapArr[0];
		heap->heapArr[0] = heap->heapArr[heap->last];
		heap->heapArr [ heap->last] = '\0';
		heap->last--;
		_reheapDown(heap, 0);
		return 1;
	}
};











/* Print heap array
그림 예시처럼 화면에 출력되게하는 함수
*/
void heapPrint(HEAP *heap)
{
	if (heap->last != -1)
	{
		int i = 0;
		while (i != heap->last + 1)
		{
			printf("\t%d", *(heap->heapArr + i));
			i++;
		}
	}
	printf("\n");
};








//랜덤하게 총 20개의 숫자를 생성
int main(void)
{
	HEAP *heap;
	int data;
	int i;

	heap = heapCreate(MAX_ELEM);

	srand(time(NULL));

	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % MAX_ELEM * 3 + 1; // 1 ~ MAX_ELEM*3 random number

		fprintf(stdout, "Inserting %d: ", data);

		// insert function call
		heapInsert(heap, data);

		heapPrint(heap);
	}

	while (heap->last >= 0)
	{
		 //delete function call
		 heapDelete( heap, &data);

		printf( "Deleting %d: ", data);

		heapPrint( heap);
	}

	heapDestroy( heap);

	return 0;
}

