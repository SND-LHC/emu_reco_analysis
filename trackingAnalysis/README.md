# Tracking and Reconstruction Optimization Study 

This directory contains analysis scripts for studying optimization of segment and vertex reconstruction efficiency 
using muon and neutrino MC simulations as well as data from Run 1, b000044, cell (8, 13). These scripts can be modified to future analyses
to check reconstruction improvements and issues. 

## Overview of Scripts

- RealData_v_MC.ipynb
    - generate plots of interesting variables from data, as well as comparison to MC simulations of muons and neutrinos
- MixedSim_FeasibilityAnalysis.ipynb
    - analysis of neutrino MC, exploring feasibility of a mixed neutrino and muon simulation which is not too computationally intensive
- trackingQuality_muonMC
    - analysis of different cuts on tracking of muons from muon MC simulation (reduced density, reduced smearing) to understand effects on
      segment reconstruction efficiency
- trackingQuality_angleCuts
    - analysis of cuts on shallow-angled base tracks in muon and neutrino MC as well as real data, to see how vertex and segment reconstruction
      efficiencies are altered
- AngleCuts_EllipticalDataFits
    - derive equations of best-fit elliptical cuts at 3 chosen confidence levels (standard deviations) from the mean of base track angular
      distribution of data
    - these cuts can be applied directly to the track.rootrc file prior to tracking and vertexing
- BDT_angleCuts
    - analysis of BDT output given different angular cuts
    - see how signal vs. background (using best cut value from Fabio's work) are affected by different base track angular cuts, and verify whether
      this method actually improves signal-to-background ratio

Note: These scripts take as input the track and/or vertex root files and use uproot to perform the analyses in python. These scripts can be
used to reproduce the figures presented in my work throughout the summer, and please don't hesitate to reach out if you have any additional questions!
