#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef long BIG;

typedef struct Vertex {
    BIG id, way;
    double weight;
    int key;

    struct Vertex *next;
}   Vertex;

typedef struct {
    struct Vertex *head;
} Edge;

typedef struct {
    BIG V;
    Edge *arr;
} Graph;

// Data

typedef struct {
    int key;
    int way;
    char *name;
} STREET;

typedef struct {
    int key;
    double lat, lon;
    BIG id, way;
} ID;

// Queue
#define MAX 50000

typedef struct pqueue
{
    int data[MAX];
    int rear,front;
}pqueue;

void initialize(pqueue *p);
int empty(pqueue *p);
int full(pqueue *p);
void enqueue(pqueue *p, int x);
int dequeue(pqueue *p);
void qprint(pqueue *p);

void initialize(pqueue *p)
{
    p->rear=-1;
    p->front=-1;
}

int empty(pqueue *p)
{
    if(p->rear==-1)
        return(1);

    return(0);
}

int full(pqueue *p)
{
    if((p->rear+1)%MAX==p->front)
        return(1);

    return(0);
}

void enqueue(pqueue *p, int x)
{
    int i;
    if(full(p))
        printf("\nOverflow");
    else
    {
        if(empty(p))
        {
            p->rear=p->front=0;
            p->data[0]=x;
        }
        else
        {
            i=p->rear;

            while(x>p->data[i])
            {
                p->data[(i+1)%MAX]=p->data[i];
                i=(i-1+MAX)%MAX; //anticlockwise movement inside the queue
                if((i+1)%MAX==p->front)
                    break;
            }

            //insert x
            i=(i+1)%MAX;
            p->data[i]=x;

            //re-adjust rear
            p->rear=(p->rear+1)%MAX;
        }
    }
}

int dequeue(pqueue *p)
{
    int x;

    if(empty(p))
    {
        printf("\nUnderflow..");
    }
    else
    {
        x=p->data[p->front];
        if(p->rear==p->front)   //delete the last element
            initialize(p);
        else
            p->front=(p->front +1)%MAX;
    }

    return(x);
}

void qprint(pqueue *p)
{
    int i,x;

    if(empty(p))
    {
        printf("\nQueue is empty..");
    }
    else
    {
        i=p->front;

        while(i!=p->rear)
        {
            x=p->data[i];
            printf("\n%d",x);
            i=(i+1)%MAX;
        }
        x=p->data[i];
        printf("\n%d",x);
    }
}

//

FILE *opener(char *name , char *how)
{
    FILE *d;

    d = fopen( name , how );
    if( d == NULL)
    {
      exit(EXIT_FAILURE);
    };

    return d;
}
