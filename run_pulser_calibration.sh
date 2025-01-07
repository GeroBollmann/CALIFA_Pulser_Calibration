#!/bin/bash

# Run ROOT script with different function arguments: create spectrum_gamma.root, spectrum_proton.root and then run pulser calibration

root -l -q -b 'califa_calibParFinder_v3.C("spectrum_gamma")'

root -l -q -b 'califa_calibParFinder_v3.C("spectrum_proton")'

root -l -q -b 'califa_calibParFinder_v3.C("pulser_calibration")'
