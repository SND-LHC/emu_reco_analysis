#example: rsync --stats -rlptDvxW  /drives/d/SND/RUN3/ snd2cern@lxplus.cern.ch:/eos/experiment/sndlhc/emulsionData/2022/CERN/SND/RUN3/ &> /drives/d/rsync_logs/2022_24_11_rsinc_cern2eos.log

#!/bin/sh

localdir=/drives/d/SND/RUN3/RUN3_W5_B1
remotedir=/eos/experiment/sndlhc/emulsionData/2022/CERN/SND_mic2/RUN3/
currdate=`date +"%Y%m%d_%s"`
cryptedfile=/drives/d/snd2cern.txt.gpg

gpg -d -q $cryptedfile | sshpass rsync --stats -rlptDvxW ${localdir} --exclude '*.obx' snd2cern@lxplus.cern.ch:${remotedir} &> /drives/d/rsync_logs/${currdate}_rsync_cern2eos.log
