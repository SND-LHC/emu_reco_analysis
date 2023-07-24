#rsync --stats -rlptDvxW  /drives/d/SND aiulian@lxplus.cern.ch:/eos/experiment/sndlhc/emulsionData/2022/CERN &> rsync_logs/2022_24_11_rsinc_cern2eos.log


#!/bin/sh

localdir=/drives/d/SND
remotedir=/eos/experiment/sndlhc/emulsionData/2022/CERN
currdate=`date +"%Y%m%d_%s"`

rsync --stats -rlptDvxW ${localdir} snd2cern@lxplus.cern.ch:${remotedir} &> rsync_logs/${currdate}_rsync_cern2eos.log
