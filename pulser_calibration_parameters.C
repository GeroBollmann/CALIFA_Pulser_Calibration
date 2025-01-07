// input root file with collected data
TString filename_gamma = "/home/e12exp/data_calibration/September_data/11_9/rootfiles/gamma_pulser_comb.root";
TString filename_proton = "/home/e12exp/data_calibration/September_data/11_9/rootfiles/proton_pulser_comb.root";

// output spectrum file with filled histograms
TString SpectrumFileName_gamma = "/home/e12exp/data_calibration/September_data/11_9/spectra/gamma_pulser_comb_spectrum.root";
TString SpectrumFileName_proton = "/home/e12exp/data_calibration/September_data/11_9/spectra/proton_pulser_comb_spectrum.root";

// output file with QC-histograms
TString outputFileName = "/home/e12exp/data_calibration/September_data/11_9/rootfiles/pulser_comb_calibrated.root";

// CALIFA output file with the parameters calibrated in keV
TString outputCalFile = "/home/e12exp/data_calibration/September_data/11_9/CalPar/pulser_comb.par";

// all errors from calibration: peak number, ...
TString PeakErrors = "/home/e12exp/data_calibration/September_data/11_9/peak_errors.txt";

// Parameters for CALIFA
TString califamapfilename = "/home/e12exp/data_calibration/param_files/califamapping_v1.par";

// CrystalID <-> angles, FEBEX SFP, ...
TString anglesfilename = "/home/e12exp/CrystalID_mapping.txt";

//__________________________________________________________________________________________________//

// source_energies in keV
Double_t source_energies[] = {1274.5};

// offset: if 0, then offset as a variable
Double_t offset_calibration = -20;

// pulser voltages in mV
Double_t voltages_gamma_values[] = {45, 252, 425};  
Double_t voltages_proton_values[] = {45, 252, 425, 1300, 3010, 5620};
