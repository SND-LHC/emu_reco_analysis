#!/bin/bash

RUN=3
remotedir="/eos/experiment/sndlhc/emulsionData/2022/CERN/SND/RUN${RUN}/"
currdate=$(date +"%Y%m%d_%s")
cryptedfile="/drives/d/snd2cern.txt.gpg"

# Decrypt the encrypted file
gpg -d -q "${cryptedfile}" | xrdcp -N -v -s -f - -snd2cern@lxplus.cern.ch:"${remotedir}" &> "/drives/d/rsync_logs/${currdate}_xrdcp_cern2eos.log"
