#ifndef UTILS_H_SGRAPH
#define UTILS_H_SGRAPH

#include "_app_params.h"
#include <math.h>

node* nodes;                 // Graph nodes
unsigned long successors_size;     // Size of the successors list
node**  successors_list;     // List of the successors (not used directly)

void reset_graph_stats();


//##############################################################################
//##############################################################################
#ifdef sgraph_page_rank

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

#endif
//##############################################################################
//##############################################################################
#ifdef sgraph_bfs

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

    void print_graph()
    {
        for ( unsigned long i=0; i<NODES; i++ )
            cout << "Node: " << i << " Distance:" << nodes[i].distance << endl;
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

#endif
//##############################################################################
//##############################################################################
#ifdef sgraph_bellman_ford

    unsigned long  weights_size;     // Size of the weights list
    unsigned long* weights_list;     // List of the weights (not used directly)

    void reset_graph_stats()
    {
        for ( unsigned long i=1; i< NODES; i++ )
        {
            nodes[i].distance = NC; // Infinity
        }
        nodes[0].distance = 0;       // node[0] is the starting node
    }

    void print_graph()
    {
        for ( unsigned long i=0; i<NODES; i++ )
            cout << "Node: " << i << " Distance:" << nodes[i].distance << endl;
        for ( unsigned long r=0; r<NODES; r++ )
        {
            cout << "Node " << r << ".weights: [ ";
            for ( unsigned long c=0; c< nodes[r].out_degree; c++ )
            {
                cout << nodes[r].weights[c] << " ";
            }
            cout << "]" << endl;
        }        
    }

    unsigned long run_golden()
    {
        unsigned long checksum = 0;
        for ( unsigned r=0; r<NODES; r++ )
            for ( unsigned long c=0; c<nodes[r].out_degree; c++ ) // for node.successors
            {
                node*u = &nodes[r];
                node*v = nodes[r].successors[c];
                unsigned long w = nodes[r].weights[c];
                if ( u->distance != NC && v->distance > u->distance + w )
                    v->distance = u->distance + w;
                checksum += w;
            }
        return checksum;
    }

#endif
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################

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

        // Kernel specific initializations
        #ifdef sgraph_teenage_follower
        nodes[i].teenager = (rand()%2)?true:false;
        #endif
    }

    #ifdef sgraph_bellman_ford
    cout << " Initializing weights_list for the weighted graph ..." << endl;
    weights_size = 0;
    weights_list = NULL;

    for ( unsigned long i=0; i<NODES; i++ )
    {
        unsigned long d = nodes[i].out_degree;
        if ( d == 0 )
            nodes[i].weights = NULL;
        else
        {
            nodes[i].weights = (unsigned long*)malloc(d*sizeof(unsigned long));
            assert (nodes[i].weights);
            weights_size += d*sizeof(unsigned long);
        }
        for ( unsigned long j=0; j<d; j++)
            nodes[i].weights[j] = rand()%MAX_WEIGHT + 1;
    }
    
    weights_list = nodes[0].weights;
    #endif

    successors_list = nodes[0].successors;
    reset_graph_stats();
}

void print_list_of_lists()
{
    for ( unsigned long r=0; r<NODES; r++ )
    {
        cout << "Node " << r << " [ ";
        for ( unsigned long c=0; c< nodes[r].out_degree; c++ )
        {
            cout << nodes[r].successors[c]->ID << " ";
        }
        cout << "]" << endl;
    }
}

#endif