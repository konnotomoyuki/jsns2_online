{
  using namespace JSNS2;

  DummyEventModule* dummy = new DummyEventModule();
  dummy->SetRunNumber(1010);
  dummy->SetFilePath(Form("%s/data/uniformPositron10MeV-Tree.root", getenv("JSNS2_HOME")));
  RawDataFileOutputModule* output = new RawDataFileOutputModule();
  output->SetPath("dump.dat");

  Processor process;
  process.Add(dummy);
  process.Add(output);
  process.Run(10000);
}
