#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define MAX 50

//key-value pair(node)
typedef struct Pair{
	char *name;//key
	int value;//data
	struct Pair *next;//linked list having same hash value
}Pair;

//hash table
typedef struct HT{
	unsigned int tablesize;
	unsigned int count; // number of elements inside the hash table
	float max_lf;//max load factor= no. of elements inside the hash table / hash table size
	unsigned int threshold;//threshold = max load factor * hash table size
	Pair **table;
}HT;

HT *create_table(unsigned int num,float lf);
void insert(HT **ht,char *key,int data);
unsigned int hash_it(char *key);
void dealloc_table(HT **ht);
int delete(HT *ht,char *key);
Pair *create_pair(char *key,int data);
int get_value(HT *ht,char *key);
HT *rehash(HT *ht);
void display_table(HT *ht);
void input_table(HT *ht);

int main(int argc,char *argv[]){
     unsigned int num ;
	 float load_factor ;

	 puts("Enter the table size:");
	 scanf("%d",&num);
	 getchar();
	 puts("Enter the load factor:");
	 scanf("%f",&load_factor);
	 getchar();
	 HT *ht = NULL;
	 ht = create_table(num,load_factor);

	 input_table(ht);
	 display_table(ht);	 
	 dealloc_table(&ht);
	 return 0;
}


HT *create_table(unsigned int num,float lf){
	if(num <= 0){
		fprintf(stderr,"invalid tablesize.\n");
		return NULL;
	}
	if(lf >=(float)num || lf <= 0.0000){
		fprintf(stderr,"invalid max load factor value.\n");
		return NULL;
	}

	HT *ht = malloc(sizeof(*ht));
	if(!ht){
		fprintf(stderr,"hash table has not allocated\n");
		return NULL;
	}
	ht->tablesize = num;
	ht->count = 0;
	ht->max_lf = lf;
	ht->threshold =(int)(ht->max_lf * ht->tablesize);
	ht->table = malloc(ht->tablesize * sizeof(*ht->table));
	if(!ht->table){
		fprintf(stderr,"table has not allocated\n");
		free(ht);
		return NULL;
	}
	for(int i = 0; i < ht->tablesize; i++){
		ht->table[i] = NULL;
	}
	return ht;
}

Pair *create_pair(char *key,int data){
	Pair *p = malloc(sizeof(*p));
	if(!p) return NULL;
	//+1 is for '\0'
	p->name = malloc(strlen(key)+1);
	if(!p->name) return NULL;
	strncpy(p->name,key,strlen(key) + 1);
	p->value = data;
	p->next = NULL;
	return p;
}

unsigned int hash_it(char *key){
	unsigned int value = 0;
	unsigned int key_length = (unsigned int)strlen(key);
	for(unsigned int i=0; i <=key_length; i++){
		value += (int)key[i] + 45;
	}
	return value;
}

void insert(HT **ht,char *key,int data){
	if(!key) return;
	if((*ht)->count > (*ht)->threshold){
		*ht = rehash(*ht);
	}
	unsigned int value = hash_it(key);
	unsigned int index = value % (*ht)->tablesize;
	if(!(*ht)->table[index]){
		(*ht)->table[index] = create_pair(key,data);
		(*ht)->count++;
	}
	else{
		Pair *cur = NULL;
		cur = (*ht)->table[index];
		while(cur->next != NULL){
			cur = cur->next;
		}
		cur->next = create_pair(key,data);
	}
	return;
}

void input_table(HT *ht){
	char c[MAX];
	int i = 1;
	while(i){
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
HT *rehash(HT *ht){
	HT *new_ht = create_table(2 * ht->tablesize, ht->max_lf);
	Pair *cur = NULL;
	for(int i = 0; i < ht->tablesize; i++){
		cur = ht->table[i];
		while(cur != NULL){
			insert(&new_ht,cur->name,cur->value);
			cur = cur->next;
		}
	}
	dealloc_table(&ht);
	return new_ht;
}

int delete(HT *ht,char *key){
	if(!key) return -404;
	int del_data;
	unsigned int value = hash_it(key);
	unsigned int index = value % ht->tablesize;
	Pair *cur = NULL;
	cur = ht->table[index];
	if(cur && !strcmp(cur->name,key)){
		ht->table[index] = cur->next;
		del_data = cur->value;
		free(cur);
		ht->count--;
		return del_data;
	}
	while(cur->next != NULL && strcmp(cur->next->name,key)){
		cur = cur->next;
	}
	if(!cur->next){
		puts("key not found\n");
		return -404;
	}
	Pair *rmv = cur->next;
	cur->next = rmv->next;
	del_data = rmv->value;
	free(rmv);
	return del_data;
}
void display_table(HT *ht){
	 Pair *cur = NULL;
	 for(int i = 0;i < ht->tablesize; i++){
		 cur = ht->table[i];
		 printf("%d : ",i+1);
		 while(cur != NULL){
			printf("(%s , %d)->",cur->name,cur->value);
			cur = cur->next;
		 }
		 puts("null\n");
	 }
}
int get_value(HT *ht,char *key){
	if(!key) return -404;
	unsigned int value = hash_it(key);
	unsigned int index = value % ht->tablesize;
	Pair *cur = NULL;
	cur = ht->table[index];
	if(cur && !strcmp(cur->name,key)){
		return cur->value;
	}
	while(cur != NULL && strcmp(cur->name,key)){
		cur = cur->next;
	}
	if(!cur){
		puts("key not found\n");
		return -404;
	}
	return cur->value;
}
void dealloc_table(HT **ht){
	HT *table = *ht;
	Pair *cur = NULL, *rmv= NULL;
	for(int i = 0; i < table->tablesize; i++){
		cur = table->table[i];
		while(cur != NULL){
			rmv = cur;
			cur = cur->next;
			free(rmv);
		}
	}
	free(table->table);
	free(table);
	*ht = NULL;
	return;
}


