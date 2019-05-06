{
  using namespace JSNS2;

  RootFileInputModule* input = new RootFileInputModule();
  input->SetPath("dump.root");
  input->Add(new EventMetaData);
  input->Add(new RawEvent);
  RecoPulseSimpleModule* reco = new RecoPulseSimpleModule();
  RecoPulseSimpleHistModule* hist = new RecoPulseSimpleHistModule();
  HistOutputModule* output = new HistOutputModule();
  output->SetPath("hist.root");

  Processor process;
  process.Add(input);
  process.Add(reco);
  process.Add(hist);
  process.Add(output);
  process.Run();
}
