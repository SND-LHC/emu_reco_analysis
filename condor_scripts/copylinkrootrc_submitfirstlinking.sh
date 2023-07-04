#!/bin/bash
cp /eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b000044/firstlink.rootrc /eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b000044/link.rootrc
condor_submit condorlinking_first.sub
