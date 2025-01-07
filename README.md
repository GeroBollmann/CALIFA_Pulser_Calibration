___________________________Instructions___________________________

------ Measurement Procedure ------

1. Setup:
- Attach a radioactive source, such as Na22, centrally in the detector for all measurements.
- Connect the pulser to the detector.

2. Verify all crystals in Go4:
- Check all crystals, and adjust detector settings if necessary.
- Adjust the pulser frequency to ensure that the source and pulser signals have similar statistics at the end.
- Keep the pulser settings (except for voltage) consistent for all measurements.

3. Perform measurements:
- Perform individual measurements for both Wixhausen and Messel sides, in both gamma and proton ranges, for each pulser signal, ensuring that the pulser is connected to only one side during each respective measurement.
- All signals used in the gamma range must also be used in the proton range with identical settings to serve as reference values.
- Measure pulser voltages directly at the pulser output and after all preamplifiers using an oscilloscope. Ensure they are identical to maintain consistent signals through the preamplifiers.
- Collect several GB of data for each measurement to ensure sufficient statistics.
- For each setting, ensure a similar data volume so that all signals have the same statistical reliability.

(Example:
- Gamma Range: 45 mV, 252 mV, 425 mV
- Proton Range: 45 mV, 252 mV, 425 mV, 1300 mV, 3010 mV, 5620 mV)

-> .lmd files for both Wixhausen and Messel sides, covering gamma and proton ranges and for each pulser setting.

<br>

------ Software Package ------

--- Order: ---

run_pulser_calibration.sh (bash script) -> califa_calibParFinder_v3.C (calls spectrum creation and calibration with specific settings) -> pulser_calibration_parameters.C (includes file names, parameters, ...)
-> R3BCalifaMapped2CrystalCalPar.cxx (main software for spectrum creation and calibration) -> ...


--- You need: ---
- all measurement_gamma_range.lmd files with source and pulser events
- all measurement_proton_range.lmd files with source and pulser events (same as in gamma range + more with higher voltage)

- califamapping_v1.par
- califa_specs.txt

Place in the same folder (or adjust file paths in the scripts):
- run_pulser_calibration.sh
- pulser_calibration_parameters.C
- califa_calibParFinder_v3.C

--- How To run the script: ---

1. If R3BRoot is successfully installed, replace califamapping_v1.par, R3BCalifaMapped2CrystalCalPar.cxx and R3BCalifaMapped2CrystalCalPar.h with the new (desired) version.

2. For both ranges, combine all .lmd files and unpack them into a .root file using the UCESB reader.

3. If needed, change fit parameters, etc., in califa_calibParFinder_v3.C.

4. Change file and folder names, source energies, pulser voltages, and offset in pulser_calibration_parameters.C.

5. Configure R3BRoot: .../build/r3broot_build/config.sh.

6. Build R3BRoot: cmake --build .../build/r3broot_build.

7. Make the bash script executable: chmod +x run_pulser_calibration.sh.

8. Run run_pulser_calibration.sh: ./run_pulser_calibration.sh.

<br><br>
___________________________Settings: pulser_calibration_parameters.C___________________________

------ File Names ------

filename_gamma, filename_proton:
- .root files with collected data from measurements
- One file for each gamma and proton range measurement. An error occurs if both are not specified correctly.
- If not existing yet, unpack .lmd files using the UCESB reader

SpectrumFileName_gamma, SpectrumFileName_proton:
- Spectrum Files with histograms filled with events from the .root files
- Created in the first step and used for calibration
- If already existing, the program will ask whether to use or overwrite it

outputFileName:
- Created during calibration, containing all filled spectra with adjusted Gaussian curves and QC (Quality Control) histograms
- If the file already exists, it will be overwritten

outputCalFile:
- Created during calibration, containing calibration parameters for gamma and proton ranges sorted by CrystalID
- If the file already exists, it will be overwritten
- Uses califamapfilename as a template, appending parameters to it

PeakErrors:
- Created during calibration
- Lists defined error messages for each CrystalID, such as excessively large sigma values for individual peaks, to facilitate checking individual histograms
- Messages can be adjusted or added in R3BCalifaMapped2CrystalCalPar.cxx

califamapfilename:
- Mapping file for correctly reading and assigning events from measurement files

anglesfilename:
- Maps each CrystalID to corresponding angles, FEBEX modules, etc.
- Can be created using CrystalID_mapping.C. Specify .root files for both ranges in the code. Events are read individually, and parameters are assigned to CrystalIDs.

<br>

------ Parameters ------

source_energies:
- Energies of the source signals in keV, used in linear regression
- Directly determines the number of source signals to be identified

offset:
- Offset for the linear regression in the gamma range with source signals
- A value must be specified
- If set to 0, the offset is variable (only possible if at least two source signals are available)

voltages_gamma_values, voltages_proton_values:
- Measured pulser voltages for each range
- Directly determines the number of signals to be identified
- Used to verify pulser linearity

<br><br>  
___________________________Settings: califa_calibParFinder_v3.C___________________________

----- Histogram Configuration -----

SetCalRange_left/right/bins, SetCalRangeP_left/right/bins:
- determines the limits and number of bins for the gamma and proton histograms

SetSourcePeaks_left/right, SetPulserPeaks_left/right, SetSourcePeaksP_left/right, SetPulserPeaksP_left/right:
- determines the limits of the two search areas for source and pulser signals for gamma and proton regions respectively (P->Proton Range)

<br>

----- Additional Parameters -----

SetMinStatistics:
- minimum number of events required for the histogram to be saved for the respective CrystalID in spectrum.root

SetNumParameterFit:
- number of calibration parameters to be saved in the .par file. Default is 2: first for offset, second for slope

SetSigma:
- width of the Gaussian fit for identified peaks

SetMaxSigma:
- if the sigma of a Gaussian fit is greater than this value -> error message

SetMinPeakEvents:
- if a Gaussian fit contains less events than this value -> error message


