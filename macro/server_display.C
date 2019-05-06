{
  using namespace JSNS2;

  gStyle->SetPadBorderMode(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetFrameLineColor(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetPadLeftMargin(0);
  gStyle->SetPadRightMargin(0);
  gStyle->SetPadTopMargin(0);
  gStyle->SetPadBottomMargin(0);

  DQMHistInputModule* input = new DQMHistInputModule();
  HitMapCanvasModule* hitmap = new HitMapCanvasModule();
  HttpServerModule* http = new HttpServerModule();
  http->SetHostname("http:9080");

  Processor process;
  process.Add(input);
  process.Add(hitmap);
  process.Add(http);
  process.Run();
}
