{
  using namespace JSNS2;

  DQMHistInputModule* input = new DQMHistInputModule();
  CalcPulseCanvasModule* canvas = new CalcPulseCanvasModule();
  HttpServerModule* http = new HttpServerModule();

  Processor process;
  process.Add(input);
  process.Add(canvas);
  process.Add(http);
  process.Run();
}
