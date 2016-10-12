#!/bin/bash
GEM5_STATISTICS=( timestamp1.host_seconds softwarestat.total_mac )

VALUES0=(  parallel_dma golden ) #parallel 

for V0 in ${VALUES0[*]}
do
	source UTILS/default_params.sh
	create_scenario "$0/$*" "$V0" "PIM + HMC"
	
	####################################
	load_model system/gem5_pim_cluster.sh
# 	load_model gem5_perf_sim.sh				# Fast simulation without debugging
#     export GEM5_DEBUGFLAGS=Fetch,Decode

    export CLUSTER_NUMSLAVECPUS=8
    export CLUSTER_KERNEL_NAME="matrix_mac"
    export CLUSTER_KERNEL_SUBNAME=$V0
    export NUM_MAC_OPERATIONS=1000000
    
    export DATA_TYPE=float           # float, uint32_t
    
	source ./smc.sh -u $*	# Update these variables in the simulation environment
	
	####################################
    #*******
    clonedir $CLUSTER_SW_DIR/resident
    run ./build.sh # Build the main resident code
    returntopwd
    export GEM5_KERNEL=$SCENARIO_CASE_DIR/resident/resident
    #*******	
	####################################

	source ./smc.sh $*
done
	
finalize_gem5_simulation

plot_bar_chart "softwarestat.total_mac" "0" "Count"
