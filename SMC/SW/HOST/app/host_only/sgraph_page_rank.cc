#include <assert.h>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
using namespace std;

#define sgraph_teenage_follower
#define NODES 10000
#define MAX_OUTDEGREE 10
#define step 1
#define PAGERANK_MAX_ITERATIONS 100
#define PAGERANK_MAX_ERROR 0.001

/****************************************************************************/

typedef struct Node
{
    float page_rank;
    float next_rank;

    /* General parameters */
    unsigned long       ID;                 // ID of the current node
    unsigned long       out_degree;         // Number of successor nodes
    struct Node** successors;         // List of successors nodes
} node;
/****************************************************************************/

node* nodes;                 // Graph nodes
unsigned long successors_size;     // Size of the successors list
node**  successors_list;     // List of the successors (not used directly)

/****************************************************************************/

    void reset_graph_stats()
    {
        for ( unsigned long i=0; i< NODES; i++ )
        {
            nodes[i].page_rank = 0;
            nodes[i].next_rank = 0;
        }
    }

    void print_graph()
    {
        for ( unsigned long i=0; i<NODES; i++ )
            cout << "Node: " << i << " OutDegree:" << nodes[i].out_degree << " PageRank:" << nodes[i].page_rank << " NextRank: " << nodes[i].next_rank << endl;
    }

    unsigned long run_golden()
    {
        for ( unsigned long i=0; i<NODES; i++ )
        {
            nodes[i].page_rank = 1.0 / NODES;
            nodes[i].next_rank = 0.15 / NODES;
        }
        unsigned long count = 0;
        float diff = 0.0;
        do {
            for ( unsigned long i=0; i<NODES; i++ )
            {
                float delta = 0.85 * nodes[i].page_rank / nodes[i].out_degree;
                for ( unsigned long j=0; j<nodes[i].out_degree; j++ ) // for node.successors
                        nodes[i].successors[j]->next_rank += delta;
            }
            diff = 0.0;
            for ( unsigned long i=0; i<NODES; i++ )
            {
                diff += fabsf(nodes[i].next_rank - nodes[i].page_rank);
                nodes[i].page_rank = nodes[i].next_rank;
                nodes[i].next_rank = 0.15 / NODES;
            }
            cout << "ITERATION: " << count << " ERROR: " << diff << endl;
            //print_graph();
        } while (++count < PAGERANK_MAX_ITERATIONS && diff > PAGERANK_MAX_ERROR);

        return (unsigned long)(diff * 1000000.0); // Convert error to fixed point integer
    }

/****************************************************************************/


void create_graph()
{
    // Initialize the list of lists
    // Later, we should read the graph from data sets
    nodes = (node*)malloc(NODES*sizeof(node));
    assert (nodes);
    #ifdef sgraph_bfs
    queue = (unsigned long*)malloc(NODES*sizeof(ulong));
    #endif
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
    cout << "(main.cpp): Create the graph with " << NODES << " nodes ..." << endl;
    create_graph();
    cout << "(main.cpp): Running the golden model ... " << endl;
    unsigned long retval = run_golden();
    cout << " Golden model returned: " << retval << endl;
    return 0;
}

