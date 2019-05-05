{
  using namespace JSNS2;

  DummyEventModule* dummy = new DummyEventModule();
  dummy->SetRunNumber(1010);
  dummy->SetFilePath("/home/usr/tkonno/jsns/EventDisplayEVE/uniformPositron10MeV-Tree.root");
  CalcPulseModule* calc = new CalcPulseModule();
  DQMHistOutputModule* output = new DQMHistOutputModule();
  output->SetPrescale(1);

  Processor process;
  process.Add(dummy);
  process.Add(calc);
  process.Add(output);
  process.Run();
}
