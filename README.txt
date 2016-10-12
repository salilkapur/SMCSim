1) SMCSim:
    SMCSim is a high-level simulation environment developed based on gem5 [1], capable of modeling an SMC device attached to a complete host System on Chip. The General Memory System [2] of gem5 in full-system simulation mode has been adopted in SMCSim. The key goal in the design of this environment is component reuse. The simulation models are built by connecting the already available components in gem5 (e.g. DMA engine, interconnects, DRAM Controllers), and some of them have been modified and extended to model other functionalities (e.g. serial links).
    * In order to build and run this environment, please refer to SECTION 3.
    * In order to better understand the simulation enviornment, please refer to SECTION 4.
    * If you have any questions regarding this environment please send an email to [erfan <dot> azarkhish <at> unibo <dot> it]
    * If you plan to use SMCSim, please acknowledge gem5 and also cite the following publication:
        Azarkhish, Erfan, et al. "Design and Evaluation of a Processing-in-Memory Architecture for the Smart Memory Cube." International Conference on Architecture of Computing Systems. Springer International Publishing, 2016.
    
1) SMC:
    Smart Memory Cube (SMC) is a fully backward compatible and modular extension to the standard HMC [6], supporting near memory computation on its Logic Base (LoB) through a high performance interconnect designed for this purpose. Cycle-accurate design and analysis of the SMC have been previously published in [4,5], while this package provides a high-level full-system simulation environment for it.
    
3) BUILD AND FIRST RUN:





4) ADDITIONAL DOCUMENTS:
    In order to understand the environment better, you can consult the following documents:
        SMC/DOC/azarkhish-arcs16-160404.pdf
            Slides for the published article in IEEE ARCS 2016 conference [3]:
        SMC/DOC/smc-scripts.pdf
            An overview of the simulation scripts:
        SMC/DOC/smc-techreport2.pdf   [Warning: not 100% aligned with this release]
            Technical Report (A Full-System Simulation Environment for the Smart Memory Cube)
        SMC/DOC/smc-techreport2.pdf   [Warning: not 100% aligned with this release]
            Technical Report (Design and Exploration of a Processing-in-Memory Architecture for the Smart Memory Cube)

REFERENCES:
    [1] http://gem5.org/
    [2] http://gem5.org/General_Memory_System
    [3] Azarkhish, Erfan, et al. "Design and Evaluation of a Processing-in-Memory Architecture for the Smart Memory Cube." International Conference on Architecture of Computing Systems. Springer International Publishing, 2016.
    [4] Azarkhish, Erfan, et al. "Logic-Base Interconnect Design for Near Memory Computing in the Smart Memory Cube." (2016).
    [5] Azarkhish, Erfan, et al. "High performance AXI-4.0 based interconnect for extensible smart memory cubes." 2015 Design, Automation & Test in Europe Conference & Exhibition (DATE). IEEE, 2015.
    [6] http://www.hybridmemorycube.org/
