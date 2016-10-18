#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NODES 10000
#define MAX_OUTDEGREE 10
#define step 1
#define BFS_MAX_ITERATIONS 10

/****************************************************************************/

typedef struct Node
{
    /* BFS */
    unsigned long distance;

    /* General parameters */
    unsigned long       ID;                 // ID of the current node
    unsigned long       out_degree;         // Number of successor nodes
    struct Node** successors;         // List of successors nodes
} node;

// queue operations (used for BFS algorithm)
#define queue_init     { head=0; tail=0; elements=0; }
#define queue_push(X)  { queue[head]=(unsigned long)X;  head++;  if (head>=NODES) head=0; elements++; }
#define queue_pop      { tail++; if (tail>=NODES) tail=0;  elements--; } 
#define queue_top      ( queue[tail] )
#define queue_empty    ( elements==0 )
#define queue_full     ( elements==(NODES-1) )

#define NC   ((unsigned long)(-1))

/****************************************************************************/

node* nodes;                 // Graph nodes
unsigned long successors_size;     // Size of the successors list
node**  successors_list;     // List of the successors (not used directly)

/****************************************************************************/
    unsigned long* queue;    // queue for BFS search
    unsigned long  head, tail, elements;

    void reset_graph_stats()
    {
        for ( unsigned long i=1; i< NODES; i++ )
        {
            nodes[i].distance = NC;
        }
        nodes[0].distance = 0;       // node[0] is the starting node
        queue_init;                  // Initialize queue
    }

    unsigned long run_golden()
    {
        // cout << "Visited 0 with distance 0" << endl;
        unsigned long total_distance = 0;

        for (int x=0; x<BFS_MAX_ITERATIONS; x++)
        {
            nodes[x].distance = 0;       // node[0] is the starting node
            queue_init;                  // Initialize queue
            queue_push(&nodes[x]);               // Push the first element
            while ( !(queue_empty) )
            {
                node* v = (node*)queue_top;
                queue_pop;
                for ( unsigned long c=0; c<v->out_degree ; c++ )
                {
                    node*succ = v->successors[c];
                    if (succ->distance == NC) // Infinite
                    {
                        succ->distance = v->distance + 1;
                        total_distance += succ->distance;
                        // cout << "Visited " << succ->ID << " with distance " << succ->distance << endl;
                        queue_push(succ);
                    }
                }
            }
        }
        return total_distance;
    }


/****************************************************************************/


void create_graph()
{
    // Initialize the list of lists
    // Later, we should read the graph from data sets
    nodes = (node*)malloc(NODES*sizeof(node));
    assert (nodes);
    queue = (unsigned long*)malloc(NODES*sizeof(unsigned long));
    successors_size = 0;
    successors_list = NULL;

    assert(MAX_OUTDEGREE < NODES);

    for ( unsigned long i=0; i<NODES; i++ )
    {
        unsigned long num_succ = (i%step==0)?(rand()%MAX_OUTDEGREE):(0);
        nodes[i].out_degree = num_succ;
        nodes[i].ID = i;
        if ( num_succ == 0 )
            nodes[i].successors = NULL;
        else
        {
            nodes[i].successors = (node**)malloc(num_succ*sizeof(node*)); 
            assert (nodes[i].successors);
            successors_size += num_succ*sizeof(node*);
        }

        /* fill the successor list of node[i] */
        unsigned long succ = rand()%NODES; // Random;
        assert(MAX_OUTDEGREE < 500);
        for ( unsigned long j=0; j<num_succ; j++ )
        {
            while (succ==i)
            {
                succ += (rand()%(NODES/500) + 1);
                if ( succ >= NODES )
                    succ -= NODES;
            }
            /* we have found a new successor which we are not already connected to */
            nodes[i].successors[j] = &nodes[succ];
        }

    }

    successors_list = nodes[0].successors;
    reset_graph_stats();
}


/************************************************/
// Main
int main(int argc, char *argv[])
{
    printf("(main.cpp): Create the graph with %d nodes\n", NODES);
    create_graph();
    printf("(main.cpp): Running the golden model ... \n");
    unsigned long retval = run_golden();
    printf(" Golden model returned: %ld\n", retval);
    return 0;
}

