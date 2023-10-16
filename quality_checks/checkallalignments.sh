#!/bin/bash

for fromplate in {21..57}
do
 root -l -b -q alignmentresiduals.C\($fromplate\)
done
