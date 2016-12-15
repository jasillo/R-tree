#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <stdbool.h>
#include "assert.h"
#include <time.h>

#define M_ 4     //numero maximo de elementos por nodo
#define m_ 2     //numero minimo de elementos por nodo
#define Dim 2   //numero de dimensiones

typedef double data_type;
typedef double dist_type;

#include "tuple.h"
#include "boundingbox.h"
#include "node.h"
//#include "heap.h"
#include "rtree.h"

void printTuples(struct Tuple *points){
    if (!points)
        return;
	printf("(");
    int i;
    for (i=0 ; i<Dim ; i++){
   		printf("%g;", points->values[i]);
   	}
   	printf(")");
}

void print(struct Node* p,int level){
    if (!p)
        return;
    if (p->leaf){
        int i;
        struct Node_h *q = (struct Node_h*)(p->my_nodes);
    	for (i=0 ; i<level ; i++)
            printf("  ");
        printf("[");    	
        for (i=0 ; i<p->size ; i++){
        	printTuples( q->values[i]);
        	printf(",");
        }
        printf(" size: %d,",p->size);        
        printf("]\n" );        
    }
    else{
    	int i;
    	struct Node_nh *q = (struct Node_nh*)(p->my_nodes);
        for (i=0 ; i<level ; i++)
                printf("  ");       
        printf("RECTANGULO \n");
        for (i=0 ; i<M_ ; i++)
            print(q->values[i],level+1);        
    }
}

//=============================================================================
int main (int argc, char* argv[])
{
    struct Rtree *tree = create_rtree();
    data_type a[2];
    srand(time(NULL));    
    
    int i;
    for (i=0; i<50 ; i++)
    {
        a[0] = (rand()%10000)/100.00;
        a[1] = (rand()%10000)/100.00;
        insert_tree(tree,a);
    }


    print(tree->root,0);
    destroy_rtree(tree);
       
    return 0;
}


