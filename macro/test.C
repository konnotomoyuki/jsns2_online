{
  DummyEventModule* dummy = new DummyEventModule();
  dummy->SetRunNumber(1010);
  CalcPulseModule* calc = new CalcPulseModule();
  RootOutputModule* output = new RootOutputModule();
  output->SetPath("001010.root");

  Processor process;
  process.Add(dummy);
  process.Add(calc);
  process.Add(output);
  process.Run(10000);
}
