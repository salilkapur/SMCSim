#ifndef DEFS_H_GRAPH
#define DEFS_H_GRAPH

typedef struct Node
{
    /* Average Teenage Follower */
    #ifdef sgraph_teenage_follower
        bool teenager;
        unsigned long followers;
    #endif

    /* PageRank */
    #ifdef sgraph_page_rank
        float page_rank;
        float next_rank;
    #endif

    /* BFS */
    #ifdef sgraph_bfs
        unsigned long distance;
    #endif

    /* Bellman-ford */
    #ifdef sgraph_bellman_ford
        unsigned long distance;
        unsigned long*  weights;            // List of weights (weighted graph)
    #endif

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

#endif