// gui.C

#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <RQ_OBJECT.h>

#include <IDDisplay2D.hh>

using namespace JSNS2;

class MyMainFrame : public TGMainFrame {

  RQ_OBJECT("MyMainFrame");

private:
  TRootEmbeddedCanvas *fEcanvas;
  IDDisplay2D* display;
  TTimer* timer;

public:
  MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
  virtual ~MyMainFrame();
  void DoDraw();
  Bool_t HandleTimer(TTimer*);

};

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) 
  : TGMainFrame(p,w,h) {

  // Create canvas widget
  fEcanvas = new TRootEmbeddedCanvas("Ecanvas", this, 720,720);
  AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10,10,10,1));
  // Create a horizontal frame widget with buttons
  TGHorizontalFrame *hframe = new TGHorizontalFrame(this, 720,40);
  TGTextButton *draw = new TGTextButton(hframe,"&Draw");
  draw->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
  hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX,
					   5,5,3,4));
  TGTextButton *exit = new TGTextButton(hframe,"&Exit",
					"gApplication->Terminate(0)");
  hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,
					   5,5,3,4));
  AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,
					    2,2,2,2));

  // Set a name to the main frame
  SetWindowName("Simple Gui");

  // Map all subwindows of main frame
  MapSubwindows();

  // Initialize the layout algorithm
  Resize(GetDefaultSize());

  // Map main frame
  MapWindow();
  TCanvas *fCanvas = fEcanvas->GetCanvas();
  fCanvas->cd();
  display = new IDDisplay2D("h_PMTmap_charge", "PMT charge map", "Charge [ADC sum]");
  const int nPMTs = display->GetNPMTs();
  for (int i = 0; i < nPMTs; i++) {
    display->SetBinContent(i, i);
  }
  display->Draw("colz");

  std::cout << "KONNO" << std::endl;
  timer = new TTimer(this, 5000);
  timer->TurnOn();
}

void MyMainFrame::DoDraw() {
  TCanvas *fCanvas = fEcanvas->GetCanvas();
  fCanvas->Update();
}

MyMainFrame::~MyMainFrame() {
  // Clean up used widgets: frames, buttons, layout hints
  Cleanup();
  timer->TurnOff();
}

Bool_t MyMainFrame::HandleTimer(TTimer*){
  //I never actually get this notification
  const int nPMTs = display->GetNPMTs();  
  for (int i = 0; i < nPMTs; i++) {
    display->SetBinContent(i, gRandom->Gaus(100,10));
  }
  TCanvas *fCanvas = fEcanvas->GetCanvas();
  fCanvas->cd();
  display->Draw("colz");
  fCanvas->Update();
  LogFile::debug("updated");
  
  return kTRUE; 
}

void gui() {
  // Popup the GUI...
  new MyMainFrame(gClient->GetRoot(),720,720);
}
