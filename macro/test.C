{
  using namespace JSNS2;

  DummyEventModule* dummy = new DummyEventModule();
  dummy->SetRunNumber(1010);
  dummy->SetFilePath(Form("%s/data/uniformPositron10MeV-Tree.root", getenv("JSNS2_HOME")));
  RootFileOutputModule* output = new RootFileOutputModule();
  output->SetPath("dump.root");
  //output->Exclude("RawEvent");

  Processor process;
  process.Add(dummy);
  process.Add(output);
  process.Run(100);
}
