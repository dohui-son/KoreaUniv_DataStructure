#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MAX_YEAR_DURATION   10   


typedef struct {
	 char   name[20];      
	 char   sex;         
	 int    freq[MAX_YEAR_DURATION]; 
} tName;

typedef struct {
	 int      len;        
	 int      capacity;   
	 tName    *data;      
} tNames;



void load_names( FILE *fp, int year_index, tNames *names){
    
	char ex[50];
    while(fscanf(fp,"%s",ex) != EOF){
        char *tmpname = strtok(ex, ",");
        char *tmpsex = strtok(NULL,",");
        int tmpfreq = atoi(strtok(NULL,","));
        int flag = 0;
		
        for (int i = 0; i < names->len; i++)
        {
            if (strcmp(names->data[i].name, tmpname) == 0 && names->data[i].sex == tmpsex[0])
            {
                names->data[i].freq[year_index] = tmpfreq;
                flag = 1;
                break;
            }
        }
        if( flag ){
            continue;
		}
        if( names->capacity <= names->len ){
            names->capacity *= 2;
            names->data = (tName*)realloc(names->data, sizeof(tName)*(names->capacity));
        }
        memset(names->data[names->len].freq, 0, sizeof(int)*MAX_YEAR_DURATION);
		memset(names->data[names->len].name, '\0', sizeof(char)*20);
		
        strcpy(names->data[names->len].name, tmpname);
        names->data[names->len].sex = tmpsex[0];
        names->data[names->len].freq[year_index] = tmpfreq;
        (names->len)++;
    }
};

void print_names( tNames *names, int num_year ){
	 for(int j = 0; j < names->len ;j++){
			printf("%s\t%c",names-> data[j].name, names->data[j].sex);
			
			for( int i = 0; i<num_year; i++ ){
				 printf( "\t%d",names->data[j].freq[i]);
			}
			printf("\n");
	 }   
};



int compare( const void *n1, const void *n2){
	 tName* n_1 = (tName*)n1;
	 tName* n_2 = (tName*)n2;
	 static int forcompare = 0;    

			if(n_1->name[forcompare] > n_2->name[forcompare]){
				forcompare = 0;
				return 1;
			}
			else if(n_1->name[forcompare] < n_2->name[forcompare]){
				forcompare = 0;
				return -1;
			}
			else {
				if(n_1->name[forcompare] == '\0' && n_2->name[forcompare] == '\0'){
					if(n_1->sex > n_2->sex) {
						forcompare = 0;
						return 1;
					}else{
						forcompare = 0;
						return -1;
					}
				}
				forcompare++;
				return compare(n_1, n_2);
			}	 
};




tNames *create_names(void)
{
	 tNames *pnames = (tNames *)malloc( sizeof(tNames));
	 
	 pnames->len = 0;
	 pnames->capacity = 1;
	 pnames->data = (tName *)malloc(pnames->capacity * sizeof(tName));

	 return pnames;
}


void destroy_names(tNames *pnames)
{
	 free(pnames->data);
	 pnames->len = 0;
	 pnames->capacity = 0;

	 free(pnames);
}   

int main(int argc, char **argv)
{
	 tNames *names;
	 
	 int num = 0;
	 FILE *fp;
	 int num_year = 0;
	 if (argc == 1) return 0;
	 
	 names = create_names();

	 int start_year = atoi( &argv[1][3]);
	 
	 for (int i = 1; i < argc; i++)
	 {
		num_year++;
		fp = fopen( argv[i], "r");
		assert( fp != NULL);
		
		int year = atoi( &argv[i][3]); 
		
		fprintf( stderr, "Processing [%s]..\n", argv[i]);  
		

		load_names(fp, year - start_year, names);
		 
		fclose( fp);
	 }
	 

     qsort( names->data, names->len, sizeof(tName), compare);
	 

	 print_names( names, num_year);

	 destroy_names( names);
	 
	 return 1;
}