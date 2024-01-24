#include <stdio.h>
#include <stdlib.h>

typedef struct arrQue{
    int front , rear, capacity,*arr;
}arrQue;

arrQue *Queue(int size);
int deallocate(arrQue *q);
int isFull(arrQue *q);
int isNull(arrQue *q);
void enqueue(arrQue *q,int a);
int dequeue(arrQue *q);
int mydequeue(arrQue *q);
void printque(arrQue *q);
int sizeque(arrQue *q); 

int main(int argc,char *argv[]){
    int n = 4;
    arrQue *q = Queue(n);
    enqueue(q,56);
    enqueue(q,36);
    enqueue(q,9);
    enqueue(q,58);
    mydequeue(q);
    printf("%d %d %d",q->front,q->rear,sizeque(q));
    printque(q);
    deallocate(q);
}

arrQue *Queue(int size){
    arrQue *q = (arrQue*)malloc(sizeof(*q));
    q->front = q->rear = -1;
    q->capacity = size;
    q->arr = (int*)malloc(size * sizeof(*(q->arr)));
    return q;
}
void printque(arrQue *q){
    if(isNull(q)){
        printf("empty\n");
        return;
    }
    int i;
    for(i = q->front; i <= q->rear ; i++){
        printf("%d ",q->arr[i]);
    }
    //for O(n) 
    /*for(i = 0; i <= q->rear ; i++){
       printf("%d ",q->arr[i]);
    }*/
}
int deallocate(arrQue *q){
    if(q){
        if(q->arr){
            free(q->arr);
            q->arr = NULL;
        }
        free(q);
        q = NULL;
        return 1;
    }
    else{
        return 0;
    }
}
void enqueue(arrQue *q,int a){
    if(isFull(q)){
        fprintf(stderr,"queue is full\n");
        exit(1);
    }
    (q->rear)++;
    q->arr[q->rear] = a;
    if(q->front == -1){
        q->front = 0;
    }
}
//dequeue O(1)
int dequeue(arrQue *q){
    if(isNull(q)){
        fprintf(stderr,"queue is null\n");
        exit(2);
    }
    int a = q->arr[q->front];
    if(q->front == q->rear){
        q->front = q->rear = -1;
    }
    else{
        (q->front)++;
    }
    return a;
}
//dequeue O(n)
int mydequeue(arrQue *q){
    if(q->rear == -1){
        fprintf(stderr,"queue is null\n");
        exit(2);
    }
    int a = q->arr[0];
    for(int i = 0; i <= q->rear ;i++){
        q->arr[i] = q->arr[i+1];
    }
    q->arr[q->rear] = 0;
    (q->rear)--;
}

int sizeque(arrQue *q){
    //return (q->capacity + q->rear - q->front + 1) % q->capacity;

    if(q->front != -1 && q->rear != -1){
        return (q->rear - q->front + 1);
    }
    else {
        return 0;
    } 
}
int isFull(arrQue *q){
    return (q->rear == q->capacity-1);
}
int isNull(arrQue *q){
    return (q->front == -1 || q->front > q->rear);
    //if mydequeue is used 
    //return (q->rear == -1) ;
}
