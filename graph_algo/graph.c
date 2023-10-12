#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Graph Node 
typedef struct Gnode {
    unsigned int value;//data
	unsigned int num_nb;//number of neighbors
    struct Gnode **nb;//neighbors
	bool visit;//node has been visited or not
}Gnode;

//The Graph
typedef struct Graph{
	unsigned int V;//number of vertices
	Gnode *list;//adjancey list 
}Graph;

Graph *call_list(unsigned int num);
void dfs_preorder(Graph *G);
void dfs_postorder(Graph *G);
void bfs(Graph *G);
void deallocate(Graph **S);
int dfs_topo(Graph *G,Gnode *ptr,int *a,int n);
void topo_sort(Graph *G);
void clean(Graph *G);

int main(int argc,char *argv[]){
	Graph *S = NULL;
	printf("Enter the total no. of vertices:");
	unsigned int vertices;
	scanf("%d",&vertices);
	S = call_list(vertices);
	printf("DFS: ");
	topo_sort(S);
	printf("\n");
	printf("BFS: ");
	clean(S);
	bfs(S);
	printf("\n");
	deallocate(&S);
	return 0;
}

Graph *call_list(unsigned int num){
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
	int x;
	Gnode *ptr = NULL;
	
	//setting values of Vertices
	for(int i = 0; i < G->V; i++){
		printf("\nEnter the Vertex %d:",i+1);
		scanf("%d",&x);
		G->list[i].value = x;
		G->list[i].visit = false;
	}

	//setting Vertices neighbors address
	for(int i = 0; i < G->V; i++){

		//prompt the number of neighbors
		printf("\nEnter the number of neighbors of Vertex %d:",G->list[i].value);
		scanf("%d",&x);
		G->list[i].num_nb = x;

		//allocating memory for address of neighbors
		G->list[i].nb = malloc(G->list[i].num_nb * sizeof(Gnode*));
		if(!G->list[i].nb){
			//deallocating the previously allocated memory for the neighbors
			for(int m=0; m < i;m++)
				free(G->list[m].nb);	
			free(G->list);
			free(G);
			fprintf(stderr,"neigbors memory fail to allocate\n");
			return NULL;
		}

		//prompt the neighbor value
		for(int j=0; j < G->list[i].num_nb; j++){
			printf("\nEnter neighbor %d of Vertex %d:",j+1,G->list[i].value);
			scanf("%d",&x);

			//searching for the neighbor
			for(int k=0; k < G->V; k++){
				if(G->list[k].value == x){
					ptr = &(G->list[k]);
					break;
				}
			}

			//setting the address of neighbor
			G->list[i].nb[j] = ptr;
		}

	}
	return G;
}
void dfs_preorder(Graph *G){
	int k = 0;
	Gnode *cur = NULL, **stack = malloc(G->V * sizeof(Gnode *));
	
	//if BFS has been called first
	for(int m = 0; m < G->V; m++){
		cur = &(G->list[m]);
		cur->visit = false;
	}
	
	cur = &(G->list[0]);
	
	//insert the first vertex in stack
	stack[k] = cur;
	k++;

	while(k){
		//pop 
		k--;
		cur = stack[k];
		
		//print the vertex
		if(!cur->visit){
			printf("%d ",cur->value);
			cur->visit = true;
			
			for(int j = 0;j < cur->num_nb;j++){
				
				//pushing all the neighbors of cur vertex inside the stack
				if(!cur->nb[j]->visit){
					stack[k] = cur->nb[j];
					k++;
				}
			}
		}
	}
	free(stack);
}
void dfs_postorder(Graph *G){
	int k = 0;
	Gnode *cur = NULL, **stack = malloc(G->V * sizeof(Gnode *));
	
	//if BFS has been called first
	for(int m = 0; m < G->V; m++){
		cur = &(G->list[m]);
		cur->visit = false;
	}
	
	cur = &(G->list[0]);
	
	// here visit = true (vertex is either already been printed OR it is present in the stack already
	while(k != -1){	
		int m = k;
		for(int j = 0;j < cur->num_nb;j++){

			//pushing all the unvisited neighbors of cur vertex inside the stack
			if(!cur->nb[j]->visit){
				stack[k] = cur;
				k++;
				cur->visit = true;
				cur = cur->nb[j];
				break;
			}
		}
			
		//print the vertex
		if(k == m){
			printf("%d ",cur->value);
			cur->visit = true;
			k--;
			if(k != -1){
				cur = stack[k];
				cur->visit = false;
			}
		}
		
	}
	free(stack);
}
void bfs(Graph *G){
	int i;
	Gnode *cur = NULL,*ptr = NULL;
	cur = NULL;
	for(i = 0;i < G->V;i++){
		cur = &(G->list[i]);
		if(!cur->visit){
			printf("%d ",cur->value);
			cur->visit = true;
			for(int j=0;j < G->list[i].num_nb;j++){
				ptr = cur->nb[j];
				if(!ptr->visit){
					printf("%d ",ptr->value);
					ptr->visit = true;
				}
			}
		}
	}
}
void clean(Graph *G){
	for(int p = 0; p < G->V; p++){
		G->list[p].visit= false;       
	}
}
void deallocate(Graph **S){
	Graph *G = *S;
	for(int i = 0; i < G->V;i++){
		free(G->list[i].nb);
	}
	free(G->list);
	free(G);
	*S = NULL;
}

int dfs_topo(Graph *G,Gnode *ptr,int *a,unsigned int n){
	int i = n,k = 0, m;
	Gnode *cur = NULL, **stack = malloc(G->V * sizeof(Gnode *));	
	cur = ptr;
	// here visit = true (vertex is either already been printed OR it is present in the stack already
	while(k != -1){	
		m = k;
		for(int j = 0;j < cur->num_nb;j++){

			//pushing all the unvisited neighbors of cur vertex inside the stack
			if(!cur->nb[j]->visit){
				stack[k] = cur;
				k++;
				cur->visit = true;
				cur = cur->nb[j];
				break;
			}
		}
			
		//print the vertex
		if(k == m){
			if(i >= 0){
				a[i] = cur->value;
				i--;
				cur->visit = true;
			}
			k--;
			if(k != -1){
				cur = stack[k];
				cur->visit = false;
			}
		}
		
	}
	free(stack);
	return i;
}
void topo_sort(Graph *G){
	int *a = malloc(G->V * sizeof(*a));
	int m = G->V - 1;
	Gnode *cur = NULL;
	for(int i = 0; i < G->V; i++){
		cur = &(G->list[i]);
		if(!cur->visit)
			m = dfs_topo(G,cur,a,m);
	}
	for(int i = 0; i < G->V; i++){
		printf("%d ",a[i]);
	}
	free(a);
}
