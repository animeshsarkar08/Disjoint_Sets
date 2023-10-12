//In graph indices acts as a node each 'i' represents a node each 'i' has a value, no. of neighbors, total cost, edge cost, visit tag, info. of neighbors. */
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <limits.h>

//min heap
typedef struct Heap {
    unsigned int capacity;
	int rear, *heap;
}Heap;

//Edge node
typedef struct edge{
	int cost;
	unsigned int nb;
}edge;

typedef struct Graph {
	unsigned int vertices;//no. of vertices
    char *value;//data
	unsigned int *num_nb;//number of neighbors
	int *edgecost;//cost of each edge from its parent or previous vertex
	bool *visit;//node has been visited or not
	edge **nb_of;//edges to the neigbors
	unsigned int *pqindex;//keep track of index value of each vertex inside the priority queue(heap) 
}Graph;

Graph *call_list(unsigned int num);
void deallocate(Graph **S);
void path(Graph *G,unsigned int src);
Heap *createheap (unsigned int size);
void enqueue(Heap *h,Graph *G,int insert,int a);
int dequeue(Heap *h,Graph *G);
void deallocheap(Heap **h);

int main(int argc, char *argv[]){
	Graph *S = NULL;
	unsigned int num;
	printf("Enter no. of vertices(positive):");
	scanf("%d",&num);
	getc(stdin);
	S = call_list(num);
	path(S,0);
	deallocate(&S);
	return 0;`
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
    G->edgecost = malloc(G->vertices *sizeof(*G->edgecost));
	if(!G->edgecost){
		fprintf(stderr,"edgecost array can't be allocated\n");
		free(G->value);
		free(G->num_nb);
		free(G);
		return NULL;
	}

    G->visit = malloc(G->vertices *sizeof(*G->visit));
	if(!G->visit){
		fprintf(stderr,"visit array can't be allocated\n");
		free(G->value);
		free(G->num_nb);
		free(G->edgecost);
		free(G);
		return NULL;
	}

    G->nb_of = malloc(G->vertices * sizeof(*G->nb_of));
	if(!G->nb_of){
		fprintf(stderr,"nb_of array can't be allocated\n");
		free(G->value);
		free(G->num_nb);
		free(G->edgecost);
		free(G->visit);
		free(G);
		return NULL;
	}
	G->pqindex = malloc(G->vertices * sizeof(*G->pqindex));
	if(!G->pqindex){
		fprintf(stderr,"pqindex array can't be allocated\n");
		free(G->value);
		free(G->num_nb);
		free(G->edgecost);
		free(G->visit);
		free(G->nb_of);
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
		G->edgecost[i] = INT_MAX;
		G->nb_of[i] = NULL;
		G->pqindex[i] = -1;
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
		    free(G->edgecost);
		    free(G->visit);
			free(G->pqindex);
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
void enqueue(Heap *h,Graph *G,int insert,unsigned int a){
    if(h->rear == h->capacity - 1){
        fprintf(stderr,"heap is full\n");
        return;
    }
	
	int child,parent, temp;
	
	// incrementation and assigning 
	// if insert is -1 or negative then the vertex is new
	if(insert < 0){
		h->rear = h->rear + 1;
		h->heap[h->rear] = a;
		//assigning the priority index of the newly added vertex into the heap
		G->pqindex[h->heap[h->rear]] = h->rear;
		child = h->rear;
	}
	//if the vertex is already present inside the heap we will update its position
	//insert = current position of the vertex inside the heap
	else{
		h->heap[insert] = a;
		child = insert;
	}
            
    // re-arrangement 
    while( child != 0){

		parent = (child - 1) / 2;

		//compare the parent's edgecost and the child's edgecost
        if(G->edgecost[h->heap[parent]] > G->edgecost[h->heap[child]]){

			//swaping
            temp = h->heap[child];
            h->heap[child] = h->heap[parent];
            h->heap[parent] = temp;

			/* upadating priority index of the vertices inside the heap due
			 * newly added vertex */
			G->pqindex[h->heap[parent]] = parent;
			G->pqindex[h->heap[child]] = child;

			//updating the child
            child = parent;
        }
        else return;
    }
}
int dequeue(Heap *h,Graph *G){
	if(h->rear == -1){
		fprintf(stderr,"heap is empty\n");
		return -404;
	}

    int deq = h->heap[0];
	//assigning the priority index of the dequeued vertex to its default
	G->pqindex[deq] = -1;

    int i = 0;

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

		//comparing and assigning
		if(G->edgecost[h->heap[cd_1]] <= G->edgecost[h->heap[cd_2]]){

            h->heap[i] = h->heap[cd_1];

			/* upadating priority index of the vertices inside the heap due
			 * removing the top vertex */
			G->pqindex[h->heap[i]] = i; 
			//updating 'i'
            i = cd_1;
        }
        else if(G->edgecost[h->heap[cd_1]] >= G->edgecost[h->heap[cd_2]]){
            h->heap[i] = h->heap[cd_2];

			/* upadating priority index of the vertices inside the heap due
			 * removing the top vertex */
			G->pqindex[h->heap[i]] = i; 
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
	G->pqindex[h->heap[exited_at]] = exited_at; 

	h->rear--;
    return deq;
}
void deallocheap(Heap **S){
    Heap *rmv = *h;
    if(rmv){
        if(rmv->heap){
            free(rmv->heap);
        }
        free(rmv);
        *h=NULL;
        return;
    }
}
void path(Graph *G,int src){
	Heap *heap = createheap(G->vertices - 1);
	int vertex = 0,neighbor, new_cost = 0;
	G->edgecost[src] = INT_MIN;

	//enqueue the source
	enqueue(heap,G,-1,src);

	while(heap->rear != -1){
		int vertex = dequeue(heap,G);
		G->visit[vertex] = true;

		for(int i = 0; i < G->num_nb[vertex]; i++){
			neighbor = G->nb_of[vertex][i].nb;
			new_cost = G->nb_of[vertex][i].cost;
			//updating the edgecost of the neighbor
			if(new_cost < G->edgecost[neighbor]){
				G->edgecost[neighbor] = new_cost;
				if(!G->visit[neighbor] && (G->pqindex[neighbor] >= 0))
					enqueue(heap,G,G->pqindex[neighbor],neighbor);

				//if the neighbor hasn't been visited yet enqueue it
				else if(!G->visit[neighbor] && (G->pqindex[neighbor] < 0))
					enqueue(heap,G,-1,neighbor);
			}
		}
		printf("%d -> %c\n",G->edgecost[vertex],G->value[vertex]);
	}
	deallocheap(&heap);
}
void deallocate(Graph **S){ 
	Graph *G = *S;
	for(int m=0; m < G->vertices;m++){
		free(G->nb_of[m]);	
	}
	free(G->nb_of);
	free(G->value);
	free(G->num_nb);
    free(G->edgecost);
	free(G->visit);
	free(G->pqindex);
    free(G);	
	S = NULL;
}

