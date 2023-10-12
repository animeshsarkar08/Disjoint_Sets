#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Edge node
typedef struct edge{
	int cost;
	struct Gnode *nb;//always use struct keyword when using an post-define structure
}edge;

//Graph Node 
typedef struct Gnode {
    char value;//data
	int num_nb;//number of neighbors
	int total;
	bool visit;//node has been visited or not
	edge *nb_arr;//edges to the neigbors
	struct Gnode *next;
}Gnode;

//the priority queue
typedef struct Heap {
    int capacity, rear;
	Gnode **heap;
}Heap;

//The Graph
typedef struct Graph{
	int V;//number of vertices
	Gnode *list;//adjancey list 
}Graph;

Heap *createheap (int size);
void enqueue(Heap *h,Gnode *ptr);
Gnode *dequeue(Heap *h);
void printheap(Heap *h);
void deallocheap(Heap **h);
Graph *call_list(int num);
bool isNull(Heap *h);
void dijkstra(Graph *G,Gnode *start);
void deallocate(Graph **S);
void path(Graph *G);

int main(int argc,char *argv[]){
	int num;
	printf("Enter the number of vertices:");
	scanf("%d",&num);
	getc(stdin);
	Graph *S = call_list(num);
	dijkstra(S,&(S->list[0]));
	path(S);
	printf("\n");
	deallocate(&S);
	return 0;
}
void path(Graph *G){
	Gnode *cur = &(G->list[0]);
	while(cur){
		printf("%c",cur->value);
		cur = cur->next;
	}
}

Graph *call_list(int num){
	Graph *G = malloc(sizeof(*G));
	if(!G){
		fprintf(stderr,"graph can't be allocated\n");
		return NULL;
	}
	G->V = num;
	G->list = malloc(G->V * sizeof(Gnode));
	if(!G->list){
		free(G);
		fprintf(stderr,"graph can't be allocated\n");
		return NULL;
	}
	char c;
	int x,y;
	Gnode *ptr = NULL;
	printf("Enter capital alphabets:");
	//setting values of Vertices
	for(int i = 0; i < G->V; i++){
		printf("\nEnter the Vertex %d:",i+1);
		scanf("%c",&c);
		getc(stdin);
		G->list[i].value = c;
		G->list[i].visit = false;
		G->list[i].total = -1;
		G->list[i].next = NULL;
	}

	//setting Vertices neighbors address
	for(int i = 0; i < G->V; i++){

		//prompt the number of neighbors
		printf("\nEnter the number of neighbors of Vertex %c:",G->list[i].value);
		scanf("%d",&x);
		getc(stdin);
		G->list[i].num_nb = x;

		//allocating memory for address of neighbors
		G->list[i].nb_arr = malloc(G->list[i].num_nb * sizeof(edge));
		if(!G->list[i].nb_arr){
			//deallocating the previously allocated memory for the neighbors
			for(int m=0; m < i;m++)
				free(G->list[m].nb_arr);	
			free(G->list);
			free(G);
			fprintf(stderr,"neigbors memory fail to allocate\n");
			return NULL;
		}

		//prompt the neighbor value
		for(int j=0; j < G->list[i].num_nb; j++){
			printf("\nEnter neighbor %d of Vertex %c:",j+1,G->list[i].value);
			scanf("%c",&c);
			getc(stdin);
			printf("Enter the edge weight:");
			scanf("%d",&x);
			getc(stdin);
			//searching for the neighbor
			for(int k=0; k < G->V; k++){
				if(G->list[k].value == c){
					ptr = &(G->list[k]);
					break;
				}
			}

			//setting the address of neighbor
			G->list[i].nb_arr[j].nb = ptr;//edge *e = G->list[i].nb_arr[j] ; e->nb = ptr ;e->cost = x; 
			G->list[i].nb_arr[j].cost = x;
		}

	}
	return G;
}


Heap *createheap (int size){
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
void enqueue(Heap *h,Gnode *ptr){
    if(h->rear == h->capacity - 1){
        fprintf(stderr,"heap is full\n");
        return;
    }
  /* if(h->rear == -1){
       h->rear++;
        h->heap[h->rear] = a;
        return;
    }*/
    h->rear = h->rear + 1;
    h->heap[h->rear] = ptr;
            
    int i = h->rear;
    int parent;
	Gnode *temp = NULL;
    while( i != 0){
		parent = (i - 1) / 2;
        if(h->heap[parent]->total > h->heap[i]->total){
             temp = h->heap[i];
            h->heap[i] = h->heap[parent];
            h->heap[parent] = temp;
            i = parent;
        }
        else return;
    }
}
Gnode *dequeue(Heap *h){
	if(isNull(h)){
		fprintf(stderr,"the heap is empty\n");
		return NULL;
	}
    Gnode *deq = h->heap[0];
    int i = 0;
    int cd_1, cd_2;
    while(i != h->rear+1 && 2*i+1 <= h->rear  || 2*i+2 <= h->rear ){
		cd_1 = 2*i+1 , cd_2 = 2*i+2;
		if(!h->heap[cd_1] || !h->heap[cd_2])
			break;
		else if(h->heap[cd_1]->total <= h->heap[cd_2]->total){
            h->heap[i] = h->heap[cd_1];
            i = cd_1;
        }
        else if(h->heap[cd_1]->total >= h->heap[cd_2]->total){
            h->heap[i] = h->heap[cd_2];
            i = cd_2;
        }
        else return deq;
    }
    h->heap[i] = h->heap[h->rear];
    h->rear--;
    return deq;
}
bool isNull(Heap *h){
	if(h->rear == -1) return true;
	else return false;

}
void deallocheap(Heap **h){
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
void dijkstra(Graph *G,Gnode *start){
	//creating a priority queue
	Heap *h = createheap(G->V - 1);
	Gnode *prev = NULL,*cur = start;

	//temp to access a particular edge
	edge *temp = NULL;

	//initial the start vertex total = 0
	cur->total = 0;

	//add the start vertex to the heap
	enqueue(h,cur);
	cur = NULL;
	
	int new_cost;
	bool skip;

	while(!isNull(h)){
		skip = true;

		//saving the previous node to set path
		prev = cur;
		cur = dequeue(h);

		// checking if the dequeued vertex(cur) is a neighbor of the previous vertex.
	
		/* b/c if the dequeued vertex(cur) is not a neighbor of the previous vertex
		 that means previous vertex has no path to the dequeued vertex so we can't 
		 travel previous vertex to dequeud vertex(cur).*/
		if(prev){
			for(int i=0; i<prev->num_nb; i++){
				temp = &(prev->nb_arr[i]);
				if(temp->nb == cur){
					/* if dequeued vertex(cur) is the neighbor of the prev vertex 
					 * then skip tag is change to FALSE i.e. we will not skip the 
					 * dequeued vertex(cur).*/
					skip = false;
					break;
				}
			}
			/* in case if the dequeued vertex is not a neighbor then the skip tag
			 * will remain unchanged i.e. TRUE hence the loop will start again
			 * without executing the remaining lines after that.*/
			if(skip){
				temp = NULL;
				/* assigning the cur pointer to prev so that while running the loop 
				 * again prev pointer remains unchanged.*/
				cur = prev;
				continue;
			}
		}
		for(int i=0; i<cur->num_nb; i++){
			temp = &(cur->nb_arr[i]);
			if(!temp->nb->visit){
				// calculating the new cost of vertex
				new_cost = cur->total + temp->cost;

				//if the vertex is new upadate its total cost and enqueue it.
				if(temp->nb->total == -1){
					temp->nb->total = new_cost;
					enqueue(h,temp->nb);
				}

				//if the vertex is already enqueued than upadate its total cost
				else if(new_cost < temp->nb->total){
					temp->nb->total = new_cost;
				}
				else;
			}
		}
		if(prev){
			prev->next = cur;
			cur->visit = true;
		}
	}
	//deallocating the heap
	deallocheap(&h);
}

void deallocate(Graph **S){
	Graph *G = *S;
	for(int i = 0; i < G->V;i++){
		free(G->list[i].nb_arr);
	}
	free(G->list);
	free(G);
	*S = NULL;
}

