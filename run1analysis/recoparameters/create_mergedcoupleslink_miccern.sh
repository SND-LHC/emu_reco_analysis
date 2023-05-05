RECODATAPATH=/eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b000044/
BRICKID=44
iplate=$1
PLATEFOLDER="$(printf "p%0*d" 3 $iplate)"

ln -s $RECODATAPATH/$PLATEFOLDER/$BRICKID.$1.0.0_merged.cp.root $RECODATAPATH/$PLATEFOLDER/$BRICKID.$1.0.0.cp.root
