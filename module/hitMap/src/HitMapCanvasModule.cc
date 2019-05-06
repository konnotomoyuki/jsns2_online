#include "HitMapCanvasModule.hh"

#include "DBObject.hh"
#include "FADCMapping.hh"

#include <TH1.h>
#include <TFile.h>
#include <TDirectory.h>

using namespace JSNS2;

HitMapCanvasModule::HitMapCanvasModule()
  : CanvasModule ("HitMapCanvas")
{
}

HitMapCanvasModule::~HitMapCanvasModule()
{

}

Bool_t HitMapCanvasModule::Initialize()
{
  m_c = new TCanvas("c_PMT_hitmap", "c_PMT_hitmap", 1280, 640);
  m_c->Divide(2,2);
  m_c->cd(1);
  m_PMT_charge_low = new IDDisplay2D("d_PMT_charge_low", "PMT charge map", "Charge (low) [ADC]");
  m_c->cd(3);
  m_PMT_charge_high = new IDDisplay2D("d_PMT_charge_high", "PMT charge map", "Charge (high) [ADC]");
  m_c->cd(2);
  m_PMT_timing_low = new IDDisplay2D("d_PMT_timing_low", "PMT timing map", "Hit time (low) [ns]");
  m_c->cd(4);
  m_PMT_timing_high = new IDDisplay2D("d_PMT_timing_high", "PMT timing map", "Hit time (high) [ns]");
  AddCanvas(m_c);
  return true;
}

Bool_t HitMapCanvasModule::BeginRun()
{
  //StoredObject<RawData::Event> ev;
  return true;
}

Bool_t HitMapCanvasModule::ProcessEvent()
{
  TDirectory* dir = gDirectory;
  static ULong64_t count = 0;
  m_c->cd();
  TH1* h = (TH1*)dir->FindObjectAny("h_PMT_charge_low");
  if (h) {
    for (int i = 0; i < m_PMT_charge_low->GetNPMTs(); i++) {
      m_PMT_charge_low->SetBinContent(i, h->GetBinContent(i+1));
    }
    m_c->cd(1);
    m_PMT_charge_low->Draw();
  }
  h = (TH1*)dir->FindObjectAny("h_PMT_timing_low");
  if (h) {
    for (int i = 0; i < m_PMT_timing_low->GetNPMTs(); i++) {
      m_PMT_timing_low->SetBinContent(i, h->GetBinContent(i+1));
    }
    m_c->cd(3);
    m_PMT_timing_low->Draw();
  }
  h = (TH1*)dir->FindObjectAny("h_PMT_charge_high");
  if (h) {
    for (int i = 0; i < m_PMT_charge_high->GetNPMTs(); i++) {
      m_PMT_charge_high->SetBinContent(i, h->GetBinContent(i+1));
    }
    m_c->cd(2);
    m_PMT_charge_high->Draw();
  }
  h = (TH1*)dir->FindObjectAny("h_PMT_timing_high");
  if (h) {
    for (int i = 0; i < m_PMT_timing_high->GetNPMTs(); i++) {
      m_PMT_timing_high->SetBinContent(i, h->GetBinContent(i+1));
    }
    m_c->cd(4);
    m_PMT_timing_high->Draw();
  }
  m_c->Update();
  count++;
  return true;
}

Bool_t HitMapCanvasModule::EndRun()
{
  return true;
}

Bool_t HitMapCanvasModule::Finalize()
{
  return true;
}

