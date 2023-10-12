//In graph indices acts as a node each 'i' represents a node each 'i' has a value, no. of neighbors, total cost, edge cost, visit tag, info. of neighbors. */

//NOTE :- for directed graph just comment down the if statement at line 68 and its colons

#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

//Edge node
typedef struct edge{
	int cost;
	unsigned int nb;
}edge;

//set edge
typedef struct setedge{
	unsigned int vertex;//contain the vertex(index)
	unsigned int nb_index;//contain the index of the respected edge in array G->nb_of[vertex] 
}setedge;

//min heap
typedef struct Heap {
    unsigned int capacity;
	int rear;
	setedge *heap;
}Heap;

//graph
typedef struct Graph {
	unsigned int vertices;//no. of vertices
    char *value;//data
	unsigned int *num_nb;//number of neighbors
	bool *visit;//node has been visited or not
	edge **nb_of;//edges to the neigbors
}Graph;

//disjoint set
typedef struct DJT{
	unsigned int elements;
	unsigned int numcomponents;
	int *parent;
}DJT;

Graph *call_list(unsigned int num);
void dealloc_graph(Graph **S);
DJT *createDJT(unsigned int num);
int Find(DJT *d,int id,unsigned int compress);
int Union(DJT *d,int id_a,int id_b);
bool connected(DJT *d,int a,int b);
int componentsize(DJT *d,int id);
void dealloc_set(DJT **S); 
void kruskal(Graph *G);
void sort(Graph *G,setedge *S,int last);
Heap *createheap (unsigned int size);
void enqueue(Heap *h,Graph *G,setedge a);
setedge dequeue(Heap *h,Graph *G);
void deallocheap(Heap **h);

int main(int argc, char *argv[]){
   Graph *S = NULL;
   unsigned int num;
   printf("Enter the number of vertices:");
   scanf("%d",&num);
   getc(stdin);
   S = call_list(num);
   kruskal(S);
   dealloc_graph(&S);
   return 0;

}

void kruskal(Graph *G){
	/* setedge *S = malloc(G->vertices * G->vertices * sizeof(*S)); */
	/* if(!S) return; */
	Heap *heap = createheap(G->vertices * G->vertices);
	if(!heap) return;
	int i;
	setedge input;

	for(i = 0; i < G->vertices; i++){
		for(int j = 0; j < G->num_nb[i]; j++){
			//for "DIRECTED" GRAPH COMMENT THIS AND COLONS
			if(!G->visit[i]){
				input.vertex = i;
				input.nb_index = j;
				enqueue(heap,G,input);
				/* sort(G,S,p); */
				//p++;
			}
		}
		G->visit[i] = true;
	}
	DJT *d = createDJT(G->vertices);
	if(!d) return;
	int u,v;
	setedge cur;
	i = 0;
	while(heap->rear != -1){
		/*cur = S[i];
		i++;*/
		cur = dequeue(heap,G);
		u = cur.vertex;
		v = G->nb_of[cur.vertex][cur.nb_index].nb;
		if(Union(d,u,v) >= 0){
		   printf("%c-->(%d)--> %c\n",G->value[u],G->nb_of[cur.vertex][cur.nb_index].cost,G->value[v]);
		}
	}
	dealloc_set(&d);
	deallocheap(&heap);
	//free(S);
}
void sort(Graph *G,setedge *S,int last){
	int u,v,m,n;
	setedge temp;
	while(last > 0){
		m = S[last - 1].vertex;
		n = S[last - 1].nb_index;
		u = S[last].vertex;
		v = S[last].nb_index;
		if(G->nb_of[m][n].cost > G->nb_of[u][v].cost){
			temp = S[last - 1];
			S[last - 1] = S[last];
			S[last] = temp;
		}
		last = last - 1;
	}
}

Heap *createheap (unsigned int size){
    Heap *h = malloc(sizeof(*h));
    if(!h){
        fprintf(stderr,"heap can't be allocated\n");
        return NULL;
    }
    h->capacity = size;
    h->rear = - 1;
    h->heap = malloc(h->capacity * sizeof(*(h->heap)));
    if(!h->heap){
		fprintf(stderr,"heap can't be allocated\n");
        return NULL;
    }
    return h;
}
void enqueue(Heap *h,Graph *G,setedge a){
    if(h->rear == h->capacity - 1){
        fprintf(stderr,"heap is full\n");
        return;
    }
	
	int child,parent,m,n,u,v;
	setedge temp;
	
	h->rear = h->rear + 1;
	h->heap[h->rear] = a;
	child = h->rear;
            
    // re-arrangement 
    while( child != 0){

		parent = (child - 1) / 2;

		m = h->heap[parent].vertex;
		n = h->heap[parent].nb_index;
		u = h->heap[child].vertex;
		v = h->heap[child].nb_index;
		//compare the parent's edgecost and the child's edgecost
        if(G->nb_of[m][n].cost > G->nb_of[u][v].cost){

			//swaping
            temp = h->heap[child];
            h->heap[child] = h->heap[parent];
            h->heap[parent] = temp;

			//updating the child
            child = parent;
        }
        else return;
    }
}
setedge dequeue(Heap *h,Graph *G){
	if(h->rear == -1){
		setedge a;
		a.vertex = -1;
		a.nb_index = -1;
		fprintf(stderr,"heap is empty\n");
		return a;
	}

    setedge deq = h->heap[0];

    int i = 0,m,n,u,v;

	// child 1, child 2 and exited_at keep track of at which node the heapifying has stopped
    int exited_at = i ,cd_1 = 2 * i + 1, cd_2 = 2 * i + 2;


/* TWO cases in which we don't have to heapify:
	 *
	 * 1) if rear = ['0' or '1'] i.e. heap has either 1 or 2 nodes inside
	 *
	 * 2) if cd_2 =< 'rear' i.e. to heapify we need both the children
	 *  existance of child_2 = " 2 * i + 2 " confirms the existance of child_1
	 *  bcuz child_1 = " 2 * i + 1 " which is 1 less than child_2 */

	//heapifying
    while(h->rear > 1 && cd_2 <= h->rear){
		m = h->heap[cd_1].vertex;
		n = h->heap[cd_1].nb_index;
		u = h->heap[cd_2].vertex;
		v = h->heap[cd_2].nb_index;

		//comparing and assigning
		if(G->nb_of[m][n].cost <= G->nb_of[u][v].cost){

            h->heap[i] = h->heap[cd_1];

			//updating 'i'
            i = cd_1;
        }
		if(G->nb_of[m][n].cost >= G->nb_of[u][v].cost){
		
            h->heap[i] = h->heap[cd_2];

			//updating 'i'
            i = cd_2;
        }
        else;
		exited_at = i;
		cd_1 = 2*i+1; 	
		cd_2 = 2*i+2;
    }
	/* 1) rear = 0 means there is only one node 
	 * 2) exited_at =  rear means there is no node left for shifting */
	if(h->rear == 0 || exited_at == h->rear){
		h->rear--;
		return deq;
	}

	//shifting and upadating the priority index of vertices inside heap
	h->heap[exited_at] = h->heap[h->rear];

	h->rear--;
    return deq;
}

void deallocheap(Heap **S){
    Heap *rmv = *S;
    if(rmv){
        if(rmv->heap){
            free(rmv->heap);
        }
        free(rmv);
        *S=NULL;
        return;
    }
}
Graph *call_list(unsigned int num){
	Graph *G = malloc(sizeof(*G));
	if(!G){
		fprintf(stderr,"graph can't be allocated\n");
		return NULL;
	}
	G->vertices = num;
	
	G->value = malloc(G->vertices *sizeof(*G->value));
	if(!G->value){
		fprintf(stderr,"value array can't be allocated\n");
		free(G);
		return NULL;
	}
    G->num_nb = malloc(G->vertices *sizeof(*G->num_nb));
	if(!G->num_nb){
		fprintf(stderr,"num_nb array can't be allocated\n");
		free(G->value);
		free(G);
		return NULL;
	}
    G->visit = malloc(G->vertices *sizeof(*G->visit));
	if(!G->visit){
		fprintf(stderr,"visit array can't be allocated\n");
		free(G->value);
		free(G->num_nb);
		free(G);
		return NULL;
	}

    G->nb_of = malloc(G->vertices * sizeof(*G->nb_of));
	if(!G->nb_of){
		fprintf(stderr,"nb_of array can't be allocated\n");
		free(G->value);
		free(G->num_nb);
		free(G->visit);
		free(G);
		return NULL;
	}

	char c;
	int x, i, j, k;
	printf("Enter capital alphabets:");
	//setting values of Vertices
	for(i = 0; i < G->vertices; i++){
		printf("\nEnter the Vertex %d:",i+1);
		scanf("%c",&c);
		getc(stdin);
		G->value[i]= c;
		G->visit[i] = false;
		G->nb_of[i] = NULL;
	}

	//setting Vertices neighbors address
	for(i = 0; i < G->vertices; i++){

		//prompt the number of neighbors
		printf("\nEnter the number of neighbors of Vertex %c:",G->value[i]);
		scanf("%d",&x);
		getc(stdin);
		G->num_nb[i] = x;

		//allocating memory for address of neighbors
		G->nb_of[i] = malloc(G->num_nb[i] * sizeof(edge));
		if(!G->nb_of[i]){
			//deallocating the previously allocated memory for the neighbors
			for(int m=0; m < i;m++)
				free(G->nb_of[m]);	
			free(G->nb_of);
		    free(G->value);
		    free(G->num_nb);
		    free(G->visit);
		    free(G);			
			fprintf(stderr,"neigbor's memory fail to allocate\n");
			return NULL;
		}

		//prompt the neighbor value
		for(j=0; j < G->num_nb[i]; j++){
			printf("\nEnter neighbor %d of Vertex %c:",j+1,G->value[i]);
			scanf("%c",&c);
			getc(stdin);
			printf("Enter the edge weight:");
			scanf("%d",&x);
			getc(stdin);
			//searching for the neighbor
			for(k=0; k < G->vertices; k++){
				if(G->value[k] == c){
					break;
				}
			}

			//assigning the index of neighbor
			G->nb_of[i][j].nb = k; 
			G->nb_of[i][j].cost = x;
		}

	}
	return G;
}

void dealloc_graph(Graph **S){ 
	Graph *G = *S;
	for(int m=0; m < G->vertices;m++){
		free(G->nb_of[m]);	
	}
	free(G->nb_of);
	free(G->value);
	free(G->num_nb);
	free(G->visit);
    free(G);	
	S = NULL;
}

DJT *createDJT(unsigned int num){
	DJT *d = malloc(sizeof(*d));
	if(!d){
		fprintf(stderr,"disjoint can't be allocated\n");
		return NULL;
	}
	d->elements = num;
	d->numcomponents = num;
	d->parent = malloc(num * sizeof(*d->parent));
	if(!d->parent){
		//free(d->value);
		free(d);
		fprintf(stderr,"parent array can't be allocated\n");
		return NULL;
	}

	for(int i = 0; i < d->elements; i++){
		d->parent[i] = -1;
	}

	return d;
}

bool connected(DJT *d,unsigned int a,unsigned int b){
	
	//if the root nodes are equal both nodes are in same component
	return Find(d,a,0) == Find(d,b,0);
}

int componentsize(DJT *d,unsigned int id){
	
	//the component size is given by root as they are the last in chain 
	return -d->parent[Find(d,id,0)];
}

int Find(DJT *d,unsigned int cur,int compress){
	//if the cur is out of range 
	if(cur < 0 || cur >= d->elements){
		return -1;
	}
	
	unsigned int root;
	root = cur;

	//if index = parent[index] then index is the root
	while(d->parent[root] >= 0 ){
		//upgrading the root = parent of current index
		root = d->parent[root];
	}

	//path compression compress = if non-zero (run) : if zero (not run)
	if(compress){
		unsigned int parent; 
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

/* int Find_path_compress(DJT *d,int cur){ */
/* 	// if root out of range */
/* 	if(cur < 0 || cur >= d->elements){ */
/* 		return -1; */
/* 	} */
/* 	if(d->parent[cur] < 0) */
/* 		return cur; */
/* 	else */
/* 		return (d->parent[cur] = Find_path_compress(DJT *d,d->parent[cur])); */

/* } */

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

void dealloc_set(DJT **S){ 
	DJT *G = *S;
    free(G->parent);
    free(G);	
	*S = NULL;
}
