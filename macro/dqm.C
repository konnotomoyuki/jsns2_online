{
  using namespace JSNS2;

  DQMHistInputModule* dqm = new DQMHistInputModule();
  CalcPulseCanvasModule* calc = new CalcPulseCanvasModule();
  DQMCanvasOutputModule* canvas = new DQMCanvasOutputModule();
  Processor process;
  process.Add(dqm);
  process.Add(calc);
  process.Add(canvas);
  //process.Add(bout);
  process.Run();
}
