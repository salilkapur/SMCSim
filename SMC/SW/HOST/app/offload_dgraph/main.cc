#include "pim_api.hh"
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <iomanip>
#include <stdlib.h>
#include "app_utils.hh"     // This must be included before anything else
using namespace std;
#include "utils.hh"

/************************************************/
// Main
int main(int argc, char *argv[])
{
    init_region();
    srand(0);
    PIMAPI *pim = new PIMAPI(); /* Instatiate the PIM API */ 
    cout << "(main.cpp): Kernel Name: " << FILE_NAME << endl;
    cout << "(main.cpp): Create the graph with " << NODES << " nodes ..." << endl;
    create_graph();
    // print_adj_matrix();

    cout << "(main.cpp): Offloading the computation kernel ... " << endl;
    ____TIME_STAMP(1);
    #ifdef OFFLOAD_THE_KERNEL
    pim->offload_kernel((char*)FILE_NAME);
    #else
    cout << "Kernel offloading skipped! (See the OFFLOAD_THE_KERNEL environment variable)" << endl;
    #endif
    ____TIME_STAMP(2);

    /* PIM Scalar Registers
    SREG[0]: nodes
    SREG[1]: adj matrix
    SREG[2]: number of nodes
    SREG[3]: retval
    */

    ____TIME_STAMP(3);
    // Create and assign the data set for this task
    PIMTask* task = new PIMTask("graph-task");
    task->addData(nodes, NODES*sizeof(node), 0);
    task->addData(adj, NODES*NODES*sizeof(ulong_t), 1);
    pim->write_sreg(2, NODES);  // This is a parameter sent to PIM

    #ifdef dgraph_page_rank
    float e = PAGERANK_MAX_ERROR;
    pim->write_sreg(4, *((ulong_t*)&e));
    pim->write_sreg(5, PAGERANK_MAX_ITERATIONS);
    #endif

//    /* Enable packet logging */
//    //pim->give_m5_command(PIMAPI::M5_ENABLE_PACKET_LOG);

    cout << "(main.cpp): Offloading the task ... " << endl;
    pim->offload_task(task);
    ____TIME_STAMP(4);


    cout << "(main.cpp): Start computation ... " << endl;
    ____TIME_STAMP(5);
    pim->start_computation(PIMAPI::CMD_RUN_KERNEL);
    pim->wait_for_completion();
    //____TIME_STAMP(6); // This is done in the resident code

//    /* Disable packet logging */
//    //pim->give_m5_command(PIMAPI::M5_DISABLE_PACKET_LOG);

    APP_INFO("[---DONE---]")

    // Notice: the PIM kernel will touch the graph, so we must clear it again
    reset_graph_stats();

    cout << "(main.cpp): Running the golden model ... " << endl;
    ____TIME_STAMP(7);
    ulong_t retval = run_golden();
    cout << " Golden model returned: " << retval << endl;
    ____TIME_STAMP(8);

    //print_graph();

    if ( pim->read_sreg(3) == retval ) // Success
        print_happy();
    else
    {
        cout << "Error! something went wrong ... " << endl;
        cout << "      pim: " << pim->read_sreg(3) << endl;
        cout << "   golden: " << retval << endl;
    }

    pim->report_statistics();
    cout << "Exiting gem5 ..." << endl;
    pim->give_m5_command(PIMAPI::M5_EXIT);
    return 0;
}

