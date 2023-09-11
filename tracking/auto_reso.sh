python ~/sndlhc/macros-snd/run0analysis/fabio/linearfittracks.py
root -l -b -q ~/sndlhc/macros-snd/run0analysis/fabio/residualfit_resolution.C
root -l -b -q ~/sndlhc/macros-snd/run0analysis/fabio/anglefit.C
python ~/sndlhc/macros-snd/run0analysis/fabio/print_resolutions.py

