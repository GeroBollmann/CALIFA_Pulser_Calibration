typedef struct EXT_STR_h101_t {
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_CALIFA_t califa;
} EXT_STR_h101;

#include <filesystem>
#include <TString.h>
#include "pulser_calibration_parameters.C"

const Int_t nev = -1; // number of events to read, -1 - until CTRL+C
Int_t fRunId = 213;
TString cRunId = Form("%04d", fRunId);

TString function_selected;
TString filename;
TString SpectrumFileName;


//_______________________________________confirmation_______________________________________________//

bool confirmExecution() 
{
    string input;
    cout << "A spectrum file already exists. Do you want to overwrite these? (yes/no),  ";
    cin >> input;

    // Convert input to lowercase for case-insensitive comparison
    for (auto& c : input) {
        c = tolower(c);
    }

    if (input == "yes" || input == "y") {
        return true;
    } else {
        cout << "The already existing spectrum is used." << endl;
        return false;
    }
}


//________________________________________HistoFill________________________________________________//

void createSpectrum() 
{
    // Create run
    FairRunAna *run = new FairRunAna();

    // Set up R3BHeader 
    R3BEventHeader *EvntHeader = new R3BEventHeader();
    run->SetEventHeader(EvntHeader);
    //run->SetSink(new FairRootFileSink(outputFileName));

    // Runtime data base 
    FairRuntimeDb *rtdb = run->GetRuntimeDb();
    R3BFileSource *source = new R3BFileSource(filename);
    run->SetSource(source);

    // CALIFA mapping
    FairParAsciiFileIo *parIo1 = new FairParAsciiFileIo();
    parIo1->open(califamapfilename, "in");
    rtdb->setFirstInput(parIo1);
    rtdb->addRun(fRunId);
    rtdb->initContainers(fRunId);
    rtdb->print();

    R3BEventHeaderPropagator *RunIdTask = new R3BEventHeaderPropagator();
    run->AddTask(RunIdTask);
                    
    // R3BCalifaMapped2CrystalCalPar
    R3BCalifaMapped2CrystalCalPar *CalPar = new R3BCalifaMapped2CrystalCalPar();
    cout << "after creating calpar pointer" << endl;

    CalPar->SetSpectrumName(SpectrumFileName);
    CalPar->SetSourceName(function_selected);

    CalPar->SetMinStadistics(200);
    CalPar->SetNumParameterFit(2);

    // Gamma range
    CalPar->SetCalRange_left(0);
    CalPar->SetCalRange_right(30000);
    CalPar->SetCalRange_bins(3000);

    // particle range
    CalPar->SetCalRangeP_left(0);
    CalPar->SetCalRangeP_right(30000);
    CalPar->SetCalRangeP_bins(30000);

    CalPar->SetSigma(2.0);
    CalPar->SetThreshold(0.05);
    CalPar->SetMaxPeaks(20);
    CalPar->SetChi2Threshold(-3);
    CalPar->SetSigLowThreshold(3.0);
    CalPar->SetSigHighThreshold(500.);
    CalPar->SetMinWidth(1.0);
    CalPar->SetGausRange(30.0);
    CalPar->SetGausRangeP(5.0);
    CalPar->SetGausBaseEnergy(511.0);
    CalPar->SetMinSlope(1.0);
    CalPar->SetMaxSlope(1.6);
    CalPar->SetMinSlopeP(10.0);
    CalPar->SetMaxSlopeP(16.0);	  
    CalPar->SetDebugMode(0);

    cout << "before adding the task" << endl;
    run->AddTask(CalPar);
    cout << "before initialization" << endl;

    // Initialize -------------------------------------------
    run->Init();
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");   // INFO, WARNING, DEBUG

    /*
    //output Ascii file with the Calibration Parameters
    FairParAsciiFileIo *parOut = new FairParAsciiFileIo();
    parOut->open(outputCalFile, "out");
    rtdb->setOutput(parOut);

    cout << "after setting parOut" << endl;
    */
    
    if (nev > -1) {
      run->Run(nev);
    }
    else
    {
      run->Run();
    }

    //rtdb->saveOutput();

    cout << endl << endl;
    cout << "Spectrum created succesfully." << endl;
    cout << "spectrum file is " << SpectrumFileName << endl;

}


//______________________________________PulserCalibration__________________________________________________//

void pulserCalibration() 
{     

    // R3BCalifaMapped2CrystalCalPar
    R3BCalifaMapped2CrystalCalPar *CalPar = new R3BCalifaMapped2CrystalCalPar();
    cout << "after creating calpar pointer" << endl;   
 
    TArrayF* EnergythePeaks = new TArrayF(sizeof(source_energies) / sizeof(source_energies[0]));
    TArrayF* PulserVoltages_gamma = new TArrayF(sizeof(voltages_gamma_values) / sizeof(voltages_gamma_values[0]));
    TArrayF* PulserVoltages_proton = new TArrayF(sizeof(voltages_proton_values) / sizeof(voltages_proton_values[0]));

    for (int i = 0; i < EnergythePeaks->GetSize(); ++i) {
        EnergythePeaks->AddAt(source_energies[i], i);
    }

    for (int i = 0; i < PulserVoltages_gamma->GetSize(); ++i) {
        PulserVoltages_gamma->AddAt(voltages_gamma_values[i], i);
    }

    for (int i = 0; i < PulserVoltages_proton->GetSize(); ++i) {
        PulserVoltages_proton->AddAt(voltages_proton_values[i], i);
    }

    CalPar->SetEnergyPeaks(EnergythePeaks);
    cout << "after setting energy peaks" << endl;
  
    CalPar->SetPulserVoltages_gamma(PulserVoltages_gamma);
    cout << "after setting pulser voltages_gamma" << endl;
    
    CalPar->SetPulserVoltages_proton(PulserVoltages_proton);
    cout << "after setting pulser voltages_proton" << endl;    
    
    //________file_names_________//
    CalPar->SetSpectrumName_gamma(SpectrumFileName_gamma);
    CalPar->SetSpectrumName_proton(SpectrumFileName_proton);
    CalPar->SetoutputName(outputFileName);
    CalPar->SetCalName(outputCalFile);
    CalPar->SetRunId(fRunId);
    CalPar->Setcalifamapfilename(califamapfilename);
    CalPar->Setanglesfilename(anglesfilename);
    CalPar->SetSourceName(function_selected);
    CalPar->SetPeakErrors(PeakErrors);
    
    //______Gamma range______//
    // set boundaries for histogram sizes
    CalPar->SetCalRange_left(0);
    CalPar->SetCalRange_right(30000);
    CalPar->SetCalRange_bins(3000);
    
    // set boundaries for peak finding
    CalPar->SetSourcePeaks_left(200);   // NA22: if only 1 source: 500, 2: 200
    CalPar->SetSourcePeaks_right(1300);
    CalPar->SetPulserPeaks_left(1300);
    CalPar->SetPulserPeaks_right(30000);
    
    //______proton range______//
    CalPar->SetCalRangeP_left(0);
    CalPar->SetCalRangeP_right(30000);
    CalPar->SetCalRangeP_bins(30000);
    
    // set boundaries for peak finding
    CalPar->SetSourcePeaksP_left(18);   // NA22: if only 1 source: 50, 2: 18   
    CalPar->SetSourcePeaksP_right(110);
    CalPar->SetPulserPeaksP_left(110);
    CalPar->SetPulserPeaksP_right(30000);
    
    CalPar->SetOffsetCalibration(offset_calibration);
    
    //______more parameters______//
    CalPar->SetDebugMode(0);
    CalPar->SetMinStadistics(200);
    CalPar->SetNumParameterFit(2);
    
    CalPar->SetSigma(2.0);
    CalPar->SetMaxSigma(50.0);
    CalPar->SetMinPeakEvents(100.0);
    
    //call PulserCalibration function
    CalPar->PulserCalibration();

    cout << endl << endl;
    cout << "Calibration finished succesfully." << endl;
    cout << "Output file is " << outputFileName << endl;
    cout << ".par file is " << outputCalFile << endl;

}


//_______________________________________califa_calibParFinder_v3_____________________________________________________//

void califa_calibParFinder_v3(TString function) 
{
    TStopwatch timer;
    timer.Start();

    califamapfilename.ReplaceAll("//", "/");
    
    // create gamma_spectrum.root
    if (function == "spectrum_gamma")
    {
        function_selected = "spectrum";
        filename = filename_gamma;
        SpectrumFileName = SpectrumFileName_gamma;  
          
        if ((!std::filesystem::exists(SpectrumFileName.Data())) || confirmExecution())
        {
            createSpectrum(); 
        }
    }  
    
    // create proton_spectrum.root  
    else if (function == "spectrum_proton")
    {
        function_selected = "spectrum";
        filename = filename_proton;
        SpectrumFileName = SpectrumFileName_proton;
        
        if ((!std::filesystem::exists(SpectrumFileName.Data())) || confirmExecution())
        {
            createSpectrum(); 
        }    
    }
    
    // run calibration
    else if (function == "pulser_calibration")
    { 
        function_selected = "pulser";
        pulserCalibration();
    }
    
    // Finish -----------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime() / 60.;
    Double_t ctime = timer.CpuTime() / 60.;
    cout << "Real time " << rtime << " min, CPU time " << ctime << " min"
         << endl
         << endl;
    
    gApplication->Terminate();
}





