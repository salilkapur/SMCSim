#ifndef CREATE_GRAPH_TOPOLOGY
#define CREATE_GRAPH_TOPOLOGY

void create_topology()
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
    }
    successors_list = nodes[0].successors;
}

#endif
