#example: rsync --stats -rlptDvxW  /drives/d/SND/RUN1/ snd2cern@lxplus.cern.ch:/eos/experiment/sndlhc/emulsionData/2022/CERN/SND/RUN1/ &> /drives/d/rsync_logs/2022_24_11_rsinc_cern2eos.log


#!/bin/sh

localdir=/drives/d/SND/RUN1/
remotedir=/eos/experiment/sndlhc/emulsionData/2022/CERN/SND/RUN1/
currdate=`date +"%Y%m%d_%s"`

rsync --stats -rlptDvxW ${localdir} snd2cern@lxplus.cern.ch:${remotedir} &> /drives/d/rsync_logs/${currdate}_rsync_cern2eos.log
