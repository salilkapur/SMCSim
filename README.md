## 1. SMCSim:
    SMCSim is a high-level simulation environment developed based on gem5 [1],
    capable of modeling an SMC device attached to a complete host System on
    Chip. The General Memory System [2] of gem5 in full-system simulation mode
    has been adopted in SMCSim. The key goal in the design of this environment
    is component reuse. The simulation models are built by connecting the
    already available components in gem5 (e.g. DMA engine, interconnects, DRAM
    Controllers), and some of them have been modified and extended to model
    other functionalities (e.g. serial links).

    ![Alt text](SMC/DOC/smcsim-small.png?raw=true "SMCSim")

    * In order to build and run this environment, please refer to SECTION 3.
    * In order to better understand the simulation enviornment, please refer 
      to SECTION 4.
    * If you have any questions regarding this environment please send an
      email to [erfan <dot> azarkhish <at> unibo <dot> it]
    * If you plan to use SMCSim, please acknowledge gem5 and also cite the
      following publication:
        Azarkhish, Erfan, et al. "Design and Evaluation of a Processing-in-
        Memory Architecture for the Smart Memory Cube." International 
        Conference on Architecture of Computing Systems. Springer
        International Publishing, 2016.
    * This repository is not aligned with the latest version of gem5, and
      contains a clone of the development repository on 2014/10.
    
## 2. SMC:
    Smart Memory Cube (SMC) is a fully backward compatible and modular 
    extension to the standard HMC [6], supporting near memory computation on 
    its Logic Base (LoB) through a high performance interconnect designed for 
    this purpose. Cycle-accurate design and analysis of the SMC have been 
    previously published in [4,5], while this package provides a high-level 
    full-system simulation environment for it.
    
## 3. BUILD AND FIRST RUN:
    Assuming that you have cloned the following repository, you can start with
    the configurtion and build procedure:
        git@iis-git.ee.ethz.ch:erfan.azarkhish/SMCSim.git
        https://iis-git.ee.ethz.ch/erfan.azarkhish/SMCSim

    -------------------------- BUILD PROCEDURE -------------------------------
    * Move to the base directory where README.txt is located.
        cd SMSim/

    * Extract the work directory here:
        tar xvfz SMC-WORK.tgz > /dev/null

    * Configuration and Build:
        cd SMC/
        ./smc.sh --config

    * You will be prompted before building the kernels

    * Please make the following checks before continuing:
        - Required tools are already installed and "[   OK  ]" is displayed.
        - Required libraries are all installed (specially protobuf)
        - Required versions match the available versions
        - We recommend using using Ubuntu 14.04.4 LTS. All dependencies can be
          installed using apt-get.

    * "... Do you want to build the kernels now? (Y/n)"
        Press ENTER (This will build the kernels)

    * "... Do you want to build gem5 (demo scenario) now? (Y/n)"
        Press ENTER (This will build gem5 and based on a demo scenario)

    * You should see the following message:
        "ARM/gem5.opt was built successfully"

    ----------------------- RUNNING A DEMO SCENARIO---------------------------
    * Now you can run the demo simulation:
        ./scenarios/0-demo/1-singlepim-pagerank.sh -o
        Press ENTER

    * This scenario contains a single PIM device connected to an ARM host.
    A device driver for PIM is installed automatically on the embedded Linux.
    Then the PageRank task is offloaded to PIM and finally, its results are
    compared with its execution on the host side.

    * Here are some notes:
        * Instead of -o switch you can use -h to see the list of the available
          options. You can also use -b to rebuild gem5.
        * Inside another terminal you can run "./smc.sh -t" or 
          "telnet localhost 3456". This way you can attach a terminal to the
           embedded Linux.
        * This simulation should finish automatically and print the following 
          message in the telnet terminal:
            >>> Checkpointing simulation...
            >>> Exiting...

    * The first time that you run this simulation scenario, a checkpoint is
      automatically taken and the simulation exits. While, next times, the 
      simulation will resume from the checkpoint automatically and jump to the
      execution of the actual scenario (instead of booting the OS every time).

    * So, now please rerun the simulation scenario once again:
        ./scenarios/0-demo/1-singlepim-pagerank.sh -o

    * Remember that user must be in sudoers list (to mount the disk images)
      Again attach the terminal (telnet localhost 3456)
      This time, you should see the following messages in the telnet terminal:
      (Offloading and execution of the PageRank algorithm)
    
    ##############JOB START#################
    >>>> Install the driver
    ...
    >>>> Run the application and offload the kernel ...
    [PIM.API]: Offloading kernel: sgraph_page_rank.hex
    ...
    [PIM.Driver]: ...
    ...
    [PIM.API]: STARTING COMPUTATION ...
    [PIM.API]: PIM_STATUS_REG=B
    ...
    (main.cpp): Running the golden model ... 
    ITERATION: 0 ERROR: 0.606051
    ITERATION: 1 ERROR: 0.391594

    * Finally, you should see a happy smiley face in the terminal indicating 
      that the result from the PIM execution has been equal to the host side
      execution.
    Also, some statistics are reported in the main terminal:
    Stat: {sim_ticks.pim}:  9514336000
    Stat: {sim_ticks.host}:  21437286000
    ...

    * Some notes:
        * You can take a look at the simulation log files in:
          SMC-WORK/scenarios/01/sgraph_page_rank-best-000001000/m5out
          The visualization of the system is in config.dot.pdf
          The statistics reported from gem5 are in stats.txt
        * This scenario has the debugging messages enabled, so the statistics 
          are not meaningful. 
        * To run a performance simulation and gather meaningful statistics you
          should uncomment a single line in the scenario script:
          vi ./scenarios/0-demo/1-singlepim-pagerank.sh
          Uncomment this line:
          #   load_model gem5_perf_sim.sh
          Now remove the previous checkpoint:
          ./scenarios/0-demo/1-singlepim-pagerank.sh -r
          (VERY IMPORTANT: every time a hardware or firmware parameter is 
          changed the checkpoint must be deleted and retaken):
          And run the simulation again:
         ./scenarios/0-demo/1-singlepim-pagerank.sh -o
          This time, no debugging messages will be displayed and statistics 
          are meaningful.
        * The source code for the page-rank algorithm is available in:
          ./SW/PIM/kernels/sgraph_page_rank.c 

    ----------------------- AN INTERACTIVE SCENARIO---------------------------
    * Run the second demo scenario:    
        ./scenarios/0-demo/0-singlepim-manual.sh -o
        (To make sure that gem5's build is final, you can use -b beforehand)
    * This scenario is exactly similar to the previous one with the difference
      that it does not automatically run the simulation and terminate.
      So you will be able to interact with the guest OS and run the simulation
      manually
    * Attach a terminal and login with user "root" without any password.
    * Then run the following commands in the guest OS:
        cd /
        ./get
        cd work
        ./do
    * This will run the simulation same as the previous case

    ----------------------- MORE EXPLORATION SCENARIOS -----------------------
    * You can take a look at SMC/scenarios/ for a list of available scenarios
    * For example the following scenario asks PIM to execute the Average 
      Teenage Follower (ATF) kernel on a graph:
      ./scenarios/a-gem5/f-sgraph/a-teenage/1-kernel.sh 7 -b
    * You can write your own scenario and modify hardware/firmware/software
      parameters for it.
      All available parameters are defined in SMC/UTILS/default_params.sh
    * Each scenario will automatically create a directory inside the SMC-WORK/
      and store its logs and statistics to it. The name of this directory 
      corresponds with the initial characters in the scenario name.
      For example:
      ./scenarios/(a)-gem5/(f)-sgraph/(a)-teenage/(1)-kernel.sh (7) -b
      Creates:
      SMC-WORK/afa17/

## 4. ADDITIONAL DOCUMENTS:
    In order to understand the environment better, you can consult the 
    following documents:
        SMC/DOC/azarkhish-arcs16-160404.pdf
            Slides for the published article in IEEE ARCS 2016 conference [3]:

        SMC/DOC/smc-scripts.pdf
            An overview of the simulation scripts:

        SMC/DOC/smc-techreport2.pdf   [not 100% aligned with this release]
            Technical Report (A Full-System Simulation Environment for the

            Smart Memory Cube)
        SMC/DOC/smc-techreport3.pdf   [not 100% aligned with this release]
            Technical Report (Design and Exploration of a Processing-in-Memory
            Architecture for the Smart Memory Cube)

## REFERENCES:
    [1] http://gem5.org/
    [2] http://gem5.org/General_Memory_System
    [3] Azarkhish, Erfan, et al. "Design and Evaluation of a Processing-in-
        Memory Architecture for the Smart Memory Cube." International
        Conference on Architecture of Computing Systems. Springer
        International Publishing, 2016.
    [4] Azarkhish, Erfan, et al. "Logic-Base Interconnect Design for Near
        Memory Computing in the Smart Memory Cube." (2016).
    [5] Azarkhish, Erfan, et al. "High performance AXI-4.0 based interconnect
        for extensible smart memory cubes." 2015 Design, Automation & Test in
        Europe Conference & Exhibition (DATE). IEEE, 2015.
    [6] http://www.hybridmemorycube.org/
