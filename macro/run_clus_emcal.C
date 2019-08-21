void run_clus_emcal(std::string outputfile = "o2clus_emcal.root", std::string inputfile = "/opt/Data/EMCAL/job0/o2sim.root", std::string paramfile = "")
{
  // Initialize logger
  FairLogger* logger = FairLogger::GetLogger();
  logger->SetLogVerbosityLevel("LOW");
  logger->SetLogScreenLevel("DEBUG");

  // Setup timer
  TStopwatch timer;

  // Setup FairRoot analysis manager
  FairRunAna* fRun = new FairRunAna();
  FairFileSource* fFileSource = new FairFileSource(inputfile.data());
  fRun->SetSource(fFileSource);
  fRun->SetSink(new FairSink(outputfile.data()));

  // Setup Runtime DB
  /*
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(paramfile.data());
  rtdb->setFirstInput(parInput1);
  */

  // Setup clusterizer
  o2::emcal::ClusterizerParameters parameters(10000, 0, 10000, true, 0.03, 0.1, 0.05);
  o2::emcal::ClusterizerTask* clus = new o2::emcal::ClusterizerTask(&parameters); 

  fRun->AddTask(clus);

  fRun->Init();

  timer.Start();
  fRun->Run();

  std::cout << std::endl
            << std::endl;

  // Extract the maximal used memory an add is as Dart measurement
  // This line is filtered by CTest and the value send to CDash
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();

  Float_t cpuUsage = ctime / rtime;
  cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  cout << cpuUsage;
  cout << "</DartMeasurement>" << endl;
  cout << endl
       << endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outputfile.data() << std::endl;
  // std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << endl
            << endl;
}
