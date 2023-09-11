#do all steps together to evaluate resolutions
python linearfittracks.py
root -l -b -q residualfit_resolution.C
root -l -b -q anglefit.C
python print_resolutions.py

