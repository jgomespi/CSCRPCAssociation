# CSCRPCAssociation

To run, follow the commands.
To get the code:
> cmsrel CMSSW_12_0_0_pre1\
> cd CMSSW_12_0_0_pre1/src\
> cmsenv\
> git clone git@github.com:jgomespi/CSCRPCAssociation.git\
> cd CSCRPCAssociation\
> git checkout L1TriggerLevel\
> scram b -j8\
> cd CSCRPCAssociation

To generate the datasample:

> cmsRun step_0_cfg.py\
> cmsRun step_1_cfg.py\
> cmsRun step_2_cfg.py

To run the analyzer (which is at plugins/):
> cmsRun cscrpc_reco.py

