RECODATAPATH=/eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b000044/
BRICKID=44

for iplate in $(seq $2 $1)
 
 do
 PLATEFOLDER="$(printf "p%0*d" 3 $iplate)"
 ln -s $RECODATAPATH/$PLATEFOLDER/$BRICKID.$iplate.0.0_merged.cp.root $RECODATAPATH/$PLATEFOLDER/$BRICKID.$iplate.0.0.cp.root

 done
