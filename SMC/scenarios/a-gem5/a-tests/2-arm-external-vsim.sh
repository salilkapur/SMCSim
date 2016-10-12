#!/bin/bash
if ! [ -z $1 ] && ( [ $1 == "7" ] || [ $1 == "8" ] ); then echo "Arch: ARM$1"; else echo "First argument must be architecture: 7 or 8 (ARM7 or ARM8)"; exit; fi

source UTILS/default_params.sh
create_scenario "$0/$*" "default" "ARMv$1 + HMC2011 + Linux (VExpress_EMM)"
####################################
load_model memory/hmc_2011.sh
load_model system/gem5_fullsystem_arm$1.sh
load_model gem5_vsim_if.sh 1000
load_model gem5_perf_sim.sh

export GEM5_REPORT_PERIOD_ps=5000000000

# export GEM5_DEBUGFLAGS=ethz_ModelsimIF

# export GEM5_CHECKPOINT_RESTORE="TRUE"
# export GEM5_CHECKPOINT_LOCATION="/home/erfan/projects/HUGE_FILES/SMCV3.OUT/checkpoints/cpt.3909524767500"

export N_INIT_PORT=1
export DRAM_column_size="8";
export DRAM_row_size="15";


####################################
source ./smc.sh $*
print_msg "Done!"

