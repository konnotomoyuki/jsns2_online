{
  using namespace JSNS2;

  DQMHistInputModule* input = new DQMHistInputModule();
  RecoPulseSimpleCanvasModule* reco = new RecoPulseSimpleCanvasModule();
  ViewPulseCanvasModule* pulse = new ViewPulseCanvasModule();
  HttpServerModule* http = new HttpServerModule();

  Processor process;
  process.Add(input);
  process.Add(reco);
  process.Add(pulse);
  process.Add(http);
  process.Run();
}
