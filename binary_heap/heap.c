#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct Heap {
    unsigned int capacity;
	int rear, *heap;
}Heap;

Heap *createheap (unsigned int size);
void enqueue(Heap *h,int a);
int dequeue(Heap *h);
void printheap(Heap *h);
int peek(Heap *h);
void dealloc(Heap **h);

int main (int argc , char argv[]){
    Heap *h = createheap(12);

    enqueue(h,100);
    enqueue(h,99);
    enqueue(h,10);
    enqueue(h,98);
    enqueue(h,97);
    enqueue(h,8);
    enqueue(h,7);
    enqueue(h,96);
    enqueue(h,95);
    printheap(h);
	printf("\n");
    dequeue(h);
	printheap(h);
	printf("\n");
	dequeue(h);
	printheap(h);
	printf("\n");
    dealloc(&h);
	return 0;

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
void enqueue(Heap *h,int a){
    if(h->rear == h->capacity - 1){
        fprintf(stderr,"heap is full\n");
        return;
    }
	// incrementation and assigning 
    h->rear = h->rear + 1;
    h->heap[h->rear] = a;
            
    int child = h->rear,parent , temp;
    
    while( child != 0){
		parent = (child - 1) / 2;
		//compare the parent and the child
        if(h->heap[parent] > h->heap[child]){
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
int dequeue(Heap *h){
	if(h->rear == -1){
		fprintf(stderr,"heap is empty\n");
		return -404;
	}

    int deq = h->heap[0];

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
    while(h->rear > 1 &&  cd_2 <= h->rear){

		//comparing and assigning
		if(h->heap[cd_1]<= h->heap[cd_2]){

            h->heap[i] = h->heap[cd_1];

			//updating 'i'
            i = cd_1;
        }
        else if(h->heap[cd_1]>= h->heap[cd_2]){
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
	 * 2) i = rear means there is no node left for shifting */
	if(h->rear == 0 || exited_at == h->rear){
		h->rear--;
		return deq;
	}

	//shifting and upadating the priority index of vertices inside heap
	h->heap[exited_at] = h->heap[h->rear];

	h->rear--;
    return deq;
}
void printheap(Heap *h){
	for(int i=0; i < h->rear + 1; i++){
		printf("%d ",h->heap[i]);
	}

	
}

void dealloc(Heap **h){
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
