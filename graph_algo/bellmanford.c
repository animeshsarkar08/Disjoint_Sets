//In graph index acts as a node each 'i' represents a node each 'i' has a value, no. of neighbors, total cost, edge cost, visit tag, info. of neighbors. */
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <limits.h>

//Edge node
typedef struct edge{
	int cost;
	int nb;
}edge;

typedef struct Graph {
	int vertices;//no. of vertices
    char *value;//data
	int *num_nb;//number of neighbors
	int *total;//total cost to reach a vertex from the source
	bool *visit;//node has been visited or not
	edge **nb_of;//edges to the neigbors
}Graph;

Graph *call_list(int num);
void deallocate(Graph **S);
void total_cost(Graph *G,int src);
int main(int argc, char *argv[]){
	Graph *S = NULL;
	int num;
	printf("Enter no. of vertices:");
	scanf("%d",&num);
	getc(stdin);
	S = call_list(num);
	total_cost(S,0);
	for(int i=0; i  < S->vertices; i++){
		printf("%c = %d\n",S->value[i],S->total[i]);
	}
	//printf("\n");
	deallocate(&S);
	return 0;
}

Graph *call_list(int num){
	Graph *G = malloc(sizeof(*G));
	if(!G){
		fprintf(stderr,"graph can't be allocated\n");
		return NULL;
	}
	G->vertices = num;
	
	G->value = malloc(G->vertices *sizeof(G->value));
	if(!G->value){
		fprintf(stderr,"value array can't be allocated\n");
		free(G);
		return NULL;
	}
    G->num_nb = malloc(G->vertices *sizeof(G->num_nb));
	if(!G->num_nb){
		fprintf(stderr,"num_nb array can't be allocated\n");
		free(G->value);
		free(G);
		return NULL;
	}
    G->total = malloc(G->vertices *sizeof(G->total));
	if(!G->total){
		fprintf(stderr,"total array can't be allocated\n");
		free(G->value);
		free(G->num_nb);
		free(G);
		return NULL;
	}

    G->visit = malloc(G->vertices *sizeof(G->visit));
	if(!G->visit){
		fprintf(stderr,"visit array can't be allocated\n");
		free(G->value);
		free(G->num_nb);
		free(G->total);
		free(G);
		return NULL;
	}

    G->nb_of = malloc(G->vertices * sizeof(G->nb_of));
	if(!G->nb_of){
		fprintf(stderr,"nb_of array can't be allocated\n");
		free(G->value);
		free(G->num_nb);
		free(G->total);
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
		G->total[i] = INT_MAX;
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
		    free(G->total);
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

void total_cost(Graph *G,int src){
	int *queue = malloc((2 * G->vertices) * sizeof(*queue));
	int f = 0,r = 0;
	int vertex = src,neighbor, new_cost = 0;
	queue[r] = vertex;
	r++;
	G->total[src] = 0;
	bool end = true;
	int negative_tag = 0;
	while(f <= r){
		int vertex = queue[f];
		f++;
		G->visit[vertex] = true;
		for(int i = 0; i < G->num_nb[vertex]; i++){
			neighbor = G->nb_of[vertex][i].nb;
			new_cost = G->total[vertex] + G->nb_of[vertex][i].cost;
			//updating the total cost of the neighbor
			if(new_cost < G->total[neighbor]){
				G->total[neighbor] = new_cost;
				//if any changes had been made end will be assigned to FALSE
				if(end)
					end = false;
			}
			//if the neighbor hasn't been visited yet enqueue it
			if(!G->visit[neighbor]){
				queue[r] = neighbor;
				r++;
			}
		}
		//end of the a cycle
		if(f > r){
			//negative_tag keeps record of how many cycle has been completed
			negative_tag++;

			//checking if any changes occurred from previous iteration, if not then exit 
			if(end) return;
			// after negative_tag = V - 1 and if end is FALSE then there is a negative cycle 
			else if(negative_tag == (G->vertices - 1)){
				fprintf(stderr,"negative cycle is present\n");
				return;
			}
			//reseting
			f =0; r= 0;
			queue[r]  = src;
			r++;
			end = true;
			for(int i = 0; i < G->vertices; i++){
				G->visit[i] = false;
			}
		}
	}
	free(queue);
}
void deallocate(Graph **S){ 
	Graph *G = *S;
	for(int m=0; m < G->vertices;m++){
		free(G->nb_of[m]);	
	}
	free(G->nb_of);
	free(G->value);
	free(G->num_nb);
    free(G->total);
	free(G->visit);
    free(G);	
	S = NULL;
}

 

