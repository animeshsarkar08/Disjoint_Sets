//array indices acts as a node
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

typedef struct DJT{
	unsigned int elements;//total no. of elements
	unsigned int numcomponents;//total no. of componenets (i.e. total no. of root nodes) 
	char *value;//data contained inside the node(index)
	int *parent;//(mapping array) i.e. each index value is a mapping to its parent

	//theory about the parent index value of a root node 
	/*
	 * e.g. 4->3->2->1  ( 'a' is root)  || component size = 4
	 *
	 * parent[4] = 3 , parent[3] = 2, parent[2] = 1, parent[1] = 1;
	 *
	 * since instead of storing the parent[root] = root
	 * we will store the size of the component but "negative"
	 * values because we can't store 'positive value' as they are 
	 * referred as index value of parent and indices can't be 
	 * negative so if parent[node] < 0 then the node is root
	 *
	 * e.g. store the component size || "parent[1] = 4" ("+ve" value)
	 * this can be interpreted as "1->4"
	 * if "parent[1] = -4" ,since '-4' is can't be referred as an 
	 * index, size = -parent[root] */
}DJT;

DJT *createDJT(unsigned int num);
int Find(DJT *d,unsigned int id,int compress);
int Union(DJT *d,unsigned int id_a,unsigned int id_b);
unsigned int locate(DJT *d,char c);
bool connected(DJT *d,unsigned int a,unsigned int b);
unsigned int componentsize(DJT *d,unsigned int id);
void deallocate(DJT **S); 
int Find_path_compress(DJT *d,unsigned int cur);
int recur_Find(DJT *d,int cur,int compress);

int main(int argc, char *argv[]){
   DJT *s = createDJT(3);
   s->value[0] = 'A';
   s->value[1] = 'B';
   s->value[2] = 'C';
   int r= Union(s,1,2);
   printf("%d",componentsize(s,r));
   deallocate(&s);
   return 0;
}

DJT *createDJT(unsigned int num){
	DJT *d = malloc(sizeof(*d));
	if(!d){
		fprintf(stderr,"disjoint can't be allocated\n");
		return NULL;
	}
	d->elements = num;
	d->numcomponents = num;
	d->value = malloc(num * sizeof(*d->value));
	if(!d->value){
		free(d);
		fprintf(stderr,"value array can't be allocated\n");
		return NULL;
	}
	d->parent = malloc(num * sizeof(*d->parent));
	if(!d->parent){
		free(d->value);
		free(d);
		fprintf(stderr,"parent array can't be allocated\n");
		return NULL;
	}

	for(int i = 0; i < d->elements; i++){
		d->parent[i] = -1;
	}

	return d;
}

unsigned int locate(DJT *d,char c){
	int i;
	for(i = 0; i < d->elements; i++){
		if(d->value[i] == c)
			return i;
	}
	if(i == d->elements)
		return -1;
}

bool connected(DJT *d,unsigned int a,unsigned int b){
	
	//if the root nodes are equal both nodes are in same component
	return Find(d,a,0) == Find(d,b,0);
}

unsigned int componentsize(DJT *d,unsigned int id){
	
	//the component size is given by root as they are the last in chain 
	return -d->parent[Find(d,id,0)];
}

unsigned int Find(DJT *d,unsigned int cur,int compress){
	//if the cur is out of range 
	if(cur < 0 || cur >= d->elements){
		return -1;
	}
	
	int root;
	root = cur;

	//if index = parent[index] then index is the root
	while(d->parent[root] >= 0 ){
		//upgrading the root = parent of current index
		root = d->parent[root];
	}

	//path compression compress = if non-zero (run) : if zero (not run)
	if(compress){
		int parent; 
		while(d->parent[cur] != root){
			
			// root = r , parent = p, current = c   
			// c->p->r

			//saving the parent node of the current node 
			// variable = p 
			parent = d->parent[cur];

			//assigning the root as the parent node of the current node
			// c->r also p->r
			d->parent[cur] = root;
			
			//incrementing the current node to parent node
			//c = variable
			cur = parent;
		}
	}

	return root;
}
int recur_Find(DJT *d,unsigned int cur,int compress){
	//if the cur is out of range 
	if(cur < 0 ||  cur >= d->elements){
		return -1;
	}
	
	//terminating case
	if(d->parent[cur] < 0)
		return cur;
     else 
		 return recur_Find(d,d->parent[cur],0);
}

int Find_path_compress(DJT *d,unsigned int cur){
	// if root out of range
	if(cur < 0 || cur >= d->elements){
		return -1;
	}
	if(d->parent[cur] < 0)
		return cur;
	else
		return (d->parent[cur] = Find_path_compress(DJT *d,d->parent[cur]));

}

int Union(DJT *d,unsigned int id_a,unsigned int id_b){
	int root_a, root_b, size;
	root_a = Find(d,id_a,0);
	root_b = Find(d,id_b,0);
	
	//if roots are out of range
	if(root_a < 0 || root_b < 0) return -1;

	/* if the roots of both the nodes are equal this means that
	 * both the nodes are in same component*/
	if(root_a == root_b) return -404;

	//the smaller component will merge in larger component
	if(d->parent[root_a] <= d->parent[root_b]){

		size = d->parent[root_b];

		/* assigning the root of larger component as the
		 * parent of root of the smaller component */
		d->parent[root_b] = root_a;

		//upadating the size of the larger component
		d->parent[root_a] = d->parent[root_a] + size;

	   /* since two components are unified so the no. of components 
	    * is decreased by one */
     	d->numcomponents--;
		return root_a;
	}
	else {
		size = d->parent[root_a];
		d->parent[root_a] = root_b;
		d->parent[root_b] = d->parent[root_b] + size;
	    d->numcomponents--;
		return root_b;
	}
}

void deallocate(DJT **S){ 
	DJT *G = *S;
	free(G->value);
    free(G->parent);
    free(G);	
	*S = NULL;
}
