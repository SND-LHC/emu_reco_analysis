#!/bin/bash

for fromplate in {02..57}
do
 root -l -b -q alignmentresiduals.C\($fromplate\)
done
