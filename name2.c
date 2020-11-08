#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION   10   


typedef struct {
    char   name[20];      
    char   sex;         
    int      freq[MAX_YEAR_DURATION]; 
} tName;

typedef struct {
    int      len;      
    int      capacity;   
    tName   *data;      
} tNames;

int compare( const void *n1, const void *n2);
int binary_search( const void *key, const void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));

int r_they_same = 0;
void load_names( FILE *fp, int year_index, tNames *names){
    
    char ex[50];
    
    while(fscanf(fp,"%s",ex) != EOF){
     
        char *tmpname = strtok(ex, ",");
        char *tmpsex = strtok(NULL,",");
        int tmpfreq = atoi(strtok(NULL,","));
 

        tName* pnew = (tName*)malloc(sizeof(tName));

        memset(pnew->freq, 0, sizeof(int)*MAX_YEAR_DURATION);

        strcpy(pnew->name, tmpname);
        pnew->sex = tmpsex[0];
        pnew->freq[year_index] = tmpfreq;
    
        if(names->len == 0){  
                names->data[0] = *pnew;
     
                names->len++;
    
                free(pnew);
                continue;
        }
        int index = binary_search(pnew, names->data, names->len,sizeof(tName), compare); 
  
        if(r_they_same==0){ 
       
            if( names->len >= names->capacity){  
            
                names->capacity *= 2;
             
                names->data = (tName*)realloc(names->data, sizeof(tName)*(names->capacity));
       
                for (int i = (names->capacity)/2+1; i < names->capacity; ++i)
                {
          
                    memset(names->data[i].freq, 0, sizeof(int)*MAX_YEAR_DURATION);
                    memset(names->data[i].name, '\0', sizeof(char)*20);
       
                }
            }

            
            if(names->len < (index+1)){  
                names->data[index] = *pnew;
                names->len++;
                free(pnew);
                continue;
            }
            

            memmove(names->data + index + 1, names->data + index, sizeof(tName)*((names->len) - index));

            names->data[index] = *pnew;
            names->len++;
            
        }
        else{ 
            names->data[index].freq[year_index] += tmpfreq;
            r_they_same = 0;
        }
        free(pnew);
    }
};


void print_names( tNames *names, int num_year){
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
            }else if(n_1->sex < n_2->sex){
                forcompare = 0;
                return -1;
            }else{
                forcompare = 0;
                return 0;
            }
        }
        forcompare++;
        return compare(n_1, n_2);
    }
}



int binary_search( const void *key, const void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *)){

    tName* key_ptr = (tName*)key;
    tName* start_ptr = (tName*)base;

    int left = 0; 
    int right = (int)nmemb-1;
    
    while(left <= right){
    	int mid = (left+right)/2;
        if(compare(start_ptr+mid,key_ptr) == 0){
            r_they_same = 1;
            return mid;
        }
        else if(compare(start_ptr+mid,key_ptr) == 1){
            right = mid-1;
        }
        else{
            left = mid+1;
        }

    }

    return left;
    
    
}

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

        load_names( fp, year-start_year, names);
        fclose( fp);
    }

    print_names( names, num_year);

    destroy_names( names);
    
    return 1;
}