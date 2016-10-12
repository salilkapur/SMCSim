#!/bin/bash

GEM5_STATISTICS=(
"system.monitor.averageReadBandwidth"
"system.linkmons0.averageReadBandwidth"
"system.linkmons1.averageReadBandwidth"
"system.linkmons2.averageReadBandwidth"
"system.linkmons3.averageReadBandwidth"
"final_tick"
)

VALUES0=( 16 )

for V0 in ${VALUES0[*]}
do
	source UTILS/default_params.sh
	create_scenario "$0/$*" "$(zpad $V0 5)"
	####################################
	load_model memory/hmc_2011.sh
	# $1 = GEM5_TOTAL_SIMULATION_PERIOD
	# $2 = GEM5_TRAFFIC_ITT_ps
	# $3 = GEM5_TRAFFIC_NUM_TRANS
	# $4 = GEM5_TRAFFIC_ACTIVE_PERIOD_ps
	# $5 = GEM5_TRAFFIC_IDLE_PERIOD_ps	
	load_model traffic/gem5_traffic.sh -1 1000 100000 10000 1000
	####################################
	export MAX_INFLIGHT_TRANS=4096
	
	export N_INIT_PORT=$V0
	
	source ./smc.sh $*
	
done

finalize_gem5_simulation

plot_bar_chart "system.monitor.averageReadBandwidth" 0 "Bytes/S" #--no-output
print_msg "Done!"
