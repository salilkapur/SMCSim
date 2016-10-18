#include <assert.h>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <iomanip>
#include <stdlib.h>
using namespace std;

#define sgraph_teenage_follower
#define NODES 100000
#define MAX_OUTDEGREE 10
#define step 1

typedef struct Node
{
    /* Average Teenage Follower */
    bool teenager;
    unsigned long followers;

    /* General parameters */
    unsigned long       ID;                 // ID of the current node
    unsigned long       out_degree;         // Number of successor nodes
    struct Node** successors;         // List of successors nodes
} node;

node* nodes;                 // Graph nodes
unsigned long successors_size;     // Size of the successors list
node**  successors_list;     // List of the successors (not used directly)

void reset_graph_stats()
{
    for ( unsigned long i=0; i< NODES; i++ )
    {
        nodes[i].followers = 0;
    }
}

void print_graph()
{
    for ( unsigned long i=0; i<NODES; i++ )
        cout << "Node: " << i << " Teenager:" << nodes[i].teenager << " Followers: " << nodes[i].followers << endl;
}

unsigned long run_golden()
{
    for ( unsigned long r=0; r<NODES; r++ )
    {
        if ( nodes[r].teenager )
            for ( unsigned long c=0; c<nodes[r].out_degree ; c++ )
            {
                node*succ = nodes[r].successors[c];
                succ->followers++;
            }
    }
    unsigned long total_followers = 0;
    for ( unsigned long r=0; r<NODES; r++ )
        total_followers += nodes[r].followers;
    return total_followers;
}

void create_graph()
{
    // Initialize the list of lists
    // Later, we should read the graph from data sets
    nodes = (node*)malloc(NODES*sizeof(node));
    assert (nodes);
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

        // Kernel specific initializations
        nodes[i].teenager = (rand()%2)?true:false;
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

