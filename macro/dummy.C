{
  using namespace JSNS2;

  DummyEventModule* dummy = new DummyEventModule();
  dummy->SetRunNumber(1010);

  Processor process;
  process.Add(dummy);
  process.Add(output);
  process.Run();
}
