#ifndef CREATE_GRAPH_TOPOLOGY
#define CREATE_GRAPH_TOPOLOGY

#define MIN(a,b) ((a<b)?a:b)

void create_topology()
{
    srand(RANDOM_SEED);
    // Initialize the list of lists
    // Later, we should read the graph from data sets
    nodes = (node*)malloc(NODES*sizeof(node));
    assert (nodes);
    successors_list = NULL;

    unsigned long start = 0;
    unsigned long remaining = NODES;
    unsigned long component_size = 0;
    unsigned long component_degree = 0;
    for (unsigned long c=0; c < NUM_COMPONENTS; c++ )
    {
        if ( remaining == 0 )
            continue;

        if ( c == 0 )
            component_size = rand()%(remaining/2)+1;
        else if ( c == NUM_COMPONENTS-1 )
            component_size = remaining;
        else
            component_size = rand()%(remaining)+1;

        component_degree = MIN( MAX_COMPONENT_OUTDEGREE, component_size );
        printf("Component %ld -- Size: %ld -- Start: %ld -- End: %ld -- MaxDegree: %ld \n",
            c, component_size, start, start+component_size-1, component_degree  );

        for ( unsigned long n=start; n<start+component_size; n++ )
        {
            unsigned long num_succ = MIN( rand()%(component_degree+1), component_size-1);
            nodes[n].out_degree = num_succ;
            nodes[n].ID = n;
            if ( num_succ == 0 )
                nodes[n].successors = NULL;
            else
            {
                nodes[n].successors = (node**)malloc(num_succ*sizeof(node*)); 
                assert (nodes[n].successors);
            }

            /* fill the successor list of node[i] */
            unsigned long succ = start + rand()%component_size; // Random;
            for ( unsigned long j=0; j<num_succ; j++ )
            {
                if ( succ == n ) // No loop is allowed in the graph
                    succ++;
                if ( succ >= start+component_size )
                    succ = start;
                /* we have found a new successor which we are not already connected to */
                nodes[n].successors[j] = &nodes[succ];
                #ifdef DEBUG
                //printf("  node %ld --> %ld\n", n, succ );
                assert( n != succ );
                #endif
                succ ++;
            }
        }

        remaining -=component_size;
        start += component_size;
    }



    successors_list = nodes[0].successors;
}

#endif
