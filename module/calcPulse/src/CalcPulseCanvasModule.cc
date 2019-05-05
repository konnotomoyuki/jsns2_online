#include "CalcPulseCanvasModule.hh"
#include "DBObject.hh"

#include "FADCMapping.hh"

#include <TH1.h>
#include <TFile.h>
#include <TDirectory.h>

using namespace JSNS2;

CalcPulseCanvasModule::CalcPulseCanvasModule() : CanvasModule ("CalcPulseCanvas")
{
}

CalcPulseCanvasModule::~CalcPulseCanvasModule()
{

}

Bool_t CalcPulseCanvasModule::Initialize()
{
  DBObject<DB::FADCMapping> mapping("FADCMapping");
  TCanvas* c = NULL;
  for (auto& m : mapping->GetBoards()) {
    DB::FADCBoard& board(m.second);
    int i = board.GetId();
    c = new TCanvas(Form("c_FADC_%d", (int)i), Form("FADC#%d", (int)i), 640, 640);
    c->Divide(2, 2);
    m_cs.insert(std::pair<int, TCanvas*>(i, c));
    AddCanvas(c);
  }
  return true;
}

Bool_t CalcPulseCanvasModule::BeginRun()
{
  //StoredObject<RawData::Event> ev;
  return true;
}

Bool_t CalcPulseCanvasModule::ProcessEvent()
{
  DBObject<DB::FADCMapping> mapping;
  TDirectory* dir = gDirectory;
  for (auto& m : mapping->GetBoards()) {
    DB::FADCBoard& board(m.second);
    int i = board.GetId();
    TCanvas* c = m_cs[i];
    for (auto& ic : board.GetChannels()) {
      DB::FADCChannel& ch(ic.second);
      std::string hname = Form("h_pulse_PMT%d_%s", (int)ch.GetPMT(), (ch.GetGain()?"low":"high"));
      TH1* h = (TH1*)dir->FindObjectAny(hname.c_str());
      if (h != NULL) {
	c->cd((ch.GetId() / 2) + 1);
	h->SetMaximum(255);
	h->SetMinimum(0);
	h->Draw(ch.GetId()%2==0?"hist":"histsame");
      }
    }
    c->Update();
  }
  return true;
}

Bool_t CalcPulseCanvasModule::EndRun()
{
  return true;
}

Bool_t CalcPulseCanvasModule::Finalize()
{
  return true;
}

