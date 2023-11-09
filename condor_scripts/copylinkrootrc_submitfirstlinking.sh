#!/bin/bash
cp /eos/experiment/sndlhc/emulsionData/2022/emureco_Napoli/RUN3/b000044/firstlink.rootrc /eos/experiment/sndlhc/emulsionData/2022/emureco_Napoli/RUN3/b000044/link.rootrc
condor_submit condorlinking_first.sub
