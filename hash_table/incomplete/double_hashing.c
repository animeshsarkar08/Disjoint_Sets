// each index = one slot which stores a key-value pair
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#define WHILE_MAX 100

//key-value pair(node)
typedef struct Pair{
	char *name;//key
	int value;//data
	bool tombstone;//a tombstone indicates that, that particular slot has been deleted
	bool visited;// indicates the slot has been visited yet or not
}Pair;

//hash table
typedef struct HT{
	unsigned int tablesize;//HCF(probe_constant,tablesize) = 1, i.e. relatively primes 
	unsigned int count; // number of elements inside the hash table
	float max_lf;//max load factor= no. of elements inside the hash table / hash table size
	unsigned int threshold;//threshold = max load factor * hash table size
	Pair *table;
}HT;

HT *create_table();
void insert(HT **ht,char *key,int data);
int probingfun(int a,int b,int x);// probing funtion = ax + b, a != 0 and HCF(a,tablesize) = 1
unsigned int hash_1(char *key);
unsigned int hash_2(char *key);
void dealloc_table(HT **ht);
int delete(HT *ht,char *key);
void set_pair(Pair *p,char *key,int data);
Pair *get_pair(HT *ht,char *key);
HT *rehash(HT *ht);
void display_table(HT *ht);
void input_table(HT *ht);

int main(int argc,char *argv[]){
	HT *ht = create_table();
	input_table(ht);
	display_table(ht);
	dealloc_table(&ht);
}

HT *create_table(){
	HT *ht = malloc(sizeof(*ht));
	if(!ht){
		fprintf(stderr,"hash table has not allocated\n");
		return NULL;
	}
	puts("HCF(probe constant,tablesize) = 1,probe constant(coefficient of x) != 0");
	puts("Enter tablesize:");
	scanf("%d",&ht->tablesize);
	getchar();
	puts("Enter max load factor:");
	scanf("%f",&ht->max_lf);
	getchar();

	puts("Enter probing functions coefficients(ax+b):");
	if(ht->tablesize<= 0){
		fprintf(stderr,"invalid tablesize.\n");
		return NULL;
	}
	if(ht->max_lf >=(float)ht->tablesize || ht->max_lf <= 0.0000){
		fprintf(stderr,"invalid max load factor value.\n");
		return NULL;
	}

	ht->count = 0;
	ht->threshold =(int)(ht->max_lf * ht->tablesize);
	ht->table = malloc(ht->tablesize * sizeof(*ht->table));
	if(!ht->table){
		fprintf(stderr,"table has not allocated\n");
		free(ht);
		return NULL;
	}
	for(int i = 0; i < ht->tablesize; i++){
		ht->table[i].name = NULL;
		ht->table[i].tombstone = false;
		ht->table[i].visited = false;
	}
	return ht;
}
void set_pair(Pair *p,char *key,int data){
	// +1 is for '\0'
	p->name = malloc(strlen(key)+1);
	if(!p->name) return;
	strncpy(p->name,key,strlen(key) + 1);
	p->value = data;
	p->tombstone = false;
	return ;
}
unsigned int hash_1(char *key){
	unsigned int value = 0;
	unsigned int key_length = (unsigned int)strlen(key);
	for(unsigned int i=0; i <=key_length; i++){
		value += (int)key[i];
	}
	return value;
}
unsigned int hash_2(char *key){

}

int probingfun(int x,char *key,unsigned int num){
	unsigned int delta = hash_2(key) % num;
	if(delta){
		return x*delta;
	}
    else return 0;
}
void input_table(HT *ht){
	char c[MAX];
	int i = 1;
	int while_count = 1;
	while(i && while_count < WHILE_MAX){
		puts("Enter the key(name):");
		fgets(c,MAX,stdin);
		if(c[strlen(c) - 1]== '\n')
			c[strlen(c) - 1] = '\0';
		else 
			getchar();
		puts("Enter the value:");
		scanf("%d",&i);
		getchar();
		insert(&ht,c,i);
		puts("Enter the 0 to end:");
		scanf("%d",&i);
		getchar();
	}
	return;
}

void insert(HT **ht,char *key,int data){

	if(!key) return;

	if((*ht)->count == (*ht)->threshold){
		int resize;
		puts("Enter 1 to resize the table and 0 to not:");
		scanf("%d",&resize);
		getchar();
		if(!resize){
			display_table(*ht);
			exit(EXIT_SUCCESS);
		}
		*ht = rehash(*ht);
	}
	unsigned int hash_value = hash_it(key);
	unsigned int index = hash_value % (*ht)->tablesize;

	int i = 1,repeat = index;
	Pair *p = &(*ht)->table[index]; 
	if(!p->visited){
	    set_pair(p,key,data);
		(*ht)->count++;
		return;
	}
	
	/* There are 3 state of a slot we encounter while probing:
	 *
	 *  NOTE :: "C" == conditon is checked by the system
	 *			"UN" == conditon is unchecked by the system 
	 *			e.g = *(A(C) || B(UN)) if cond.A = true ,then cond.B is need not 
	 *			        to be checked.
	 *			      *(A(C) && B(C)) if cond.A = true the system will also check cond.B
	 *
	 *  THE COND. ==> " !(tombstone) && visited && strcmp(name,key) "
	 *
	 * 1) slot carries key-value pair(compare the keys and keep probing if not matched)
	 *    => "tombstone" == FALSE(C)
	 *    => "visited" == TRUE(C)
	 *    => "name" != NULL(C)
	 *
	 * 2) slot has been deleted i.e. a "TOMBSTONE"(exit the loop and set the pair at tombstone slot)
	 *    => "tombstone" == TRUE(C)
	 *    => "visited" == TRUE(UN)
	 *    => "name" == NULL(UN)
	 *
	 * 3) slot is empty i.e.not been visited yet(exit the loop and set the pair at that slot)
	 *    => "tombstone" == FALSE(C)
	 *    => "visited" == FALSE(C)
	 *    => "name" == NULL (UN)
	 *
	 *  NOTE :: whenever "name" == NULL it gets unchecked by the system.
	 *          By using this approach we completely eliminate the chance of
	 *          SEGMENTATION FAULT.*/
	for(;!p->tombstone && p->visited && strcmp(p->name,key);i++){
		index = (hash_value + probingfun(i,key,(*ht)->tabelsize)) % (*ht)->tablesize;
		if(index == repeat){
			printf("There is a cycle.Change probe constant. Set HCF(probe constant, tablesize) = 1\n");
			display_table(*ht);
			exit(EXIT_FAILURE);
		}
		p = &(*ht)->table[index]; 
	}
	//setting the new key 
	if(p->tombstone || !p->visited){
		set_pair(p,key,data);
	}
	//updating the value of an existing key
	p->value = data;
	(*ht)->count++;
	return;
}

HT *rehash(HT *ht){
	HT *new_ht = create_table();
	Pair *cur = NULL;
	for(int i = 0; i < ht->tablesize; i++){
		cur = &ht->table[i];
		insert(&new_ht,cur->name,cur->value);
	}
	dealloc_table(&ht);
	return new_ht;
}

Pair *get_pair(HT *ht,char *key){
	
	if(!key) return NULL;

	unsigned int hash_value = hash_it(key);
	unsigned int index = hash_value % ht->tablesize;

	int i;
	Pair *p = &ht->table[index];
  /* There are 3 state of a slot we encounter while probing:
	 *
	 *  NOTE :: "C" == conditon is checked by the system
	 *			"UN" == conditon is unchecked by the system 
	 *			e.g = *(A(C) || B(UN)) if cond.A = true ,then cond.B is need not 
	 *			        to be checked.
	 *			      *(A(C) && B(C)) if cond.A = true the system will also check cond.B
	 *
	 *  THE COND. ==> " tombstone || visited && strcmp(name,key) "
	 *
	 * 1) slot carries key-value pair(compare the keys and keep probing if not matched)
	 *    => "tombstone" == FALSE(C)
	 *    => "visited" == TRUE(C)
	 *    => "name" != NULL(C)
	 *
	 * 2) slot has been deleted i.e. a "TOMBSTONE" (skip the slot and keep probing)
	 *    => "tombstone" == TRUE(C)
	 *    => "visited" == TRUE(UN)
	 *    => "name" == NULL(UN)
	 *
	 * 3) slot is empty i.e.not been visited yet(we need to stop probing)
	 *    => "tombstone" == FALSE(C)
	 *    => "visited" == FALSE(C)
	 *    => "name" == NULL (UN)
	 *
	 *  NOTE :: whenever "name" == NULL it gets unchecked by the system.
	 *          By using this approach we completely eliminate the chance of
	 *          SEGMENTATION FAULT.*/

	for(i = 1;p->tombstone || (p->visited && strcmp(p->name,key));i++){
		    index = (hash_value + probingfun(ht->a,ht->b,i)) % ht->tablesize;
		p = &ht->table[index];
	}
	if(!p->visited){
		fprintf(stderr,"key not found\n");
		return NULL;
	}
	return p;
}

int delete(HT *ht,char *key){

	Pair *p = get_pair(ht,key);
	free(p->name);
	p->name = NULL;
	p->value = 0;
	p->tombstone = true;
	ht->count--;
	return p->value;

}
void display_table(HT *ht){
	 Pair *cur = NULL;
	 for(int i = 0;i < ht->tablesize; i++){
		 cur = &ht->table[i];
		 printf("%d : (%s , %d)\n",i+1,cur->name,cur->value);
	 }
}
void dealloc_table(HT **ht){
	HT *rmv_ht = *ht;
	Pair *cur = NULL;
	for(int i = 0; i < rmv_ht->tablesize; i++){
		cur = &rmv_ht->table[i];
		free(cur->name);
		cur->name = NULL;
	}
	free(rmv_ht->table);
	rmv_ht->table = NULL;
	free(rmv_ht);
	*ht = NULL;
	return;
}
