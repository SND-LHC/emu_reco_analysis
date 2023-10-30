#!/bin/bash
mkdir plot_link
mkdir plot_prelink
mkdir plot_first_align
mkdir plot_second_align
mkdir plot_local_align
mkdir trackfiles

cp /afs/cern.ch/work/s/snd2na/public/emu_reco_analysis/create_link.sh .
cp /afs/cern.ch/work/s/snd2na/public/emu_reco_analysis/linkingmap/*link.rootrc .
cp /afs/cern.ch/work/s/snd2na/public/emu_reco_analysis/linkingmap/filecheck*linking.sh .
cp /afs/cern.ch/work/s/snd2na/public/emu_reco_analysis/linkingmap/*linking_cell.sh .
cp /afs/cern.ch/work/s/snd2na/public/emu_reco_analysis/linkingmap/merge_couples*.C .
cp /afs/cern.ch/work/s/snd2na/public/emu_reco_analysis/linkingmap/prepare_align.sh .
cp /afs/cern.ch/work/s/snd2na/public/emu_reco_analysis/run1analysis/recoparameters/*_align.rootrc .
cp /afs/cern.ch/work/s/snd2na/public/emu_reco_analysis/run1analysis/recoparameters/*align.sh .
cp /afs/cern.ch/work/s/snd2na/public/emu_reco_analysis/tracking/track*.rootrc .
cp /afs/cern.ch/work/s/snd2na/public/emu_reco_analysis/tracking/tracking*_singlefiles.sh .
