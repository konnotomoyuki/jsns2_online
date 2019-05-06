#include "RecoPulseSimpleCanvasModule.hh"

#include <TH1.h>
#include <TDirectory.h>

using namespace JSNS2;

RecoPulseSimpleCanvasModule::RecoPulseSimpleCanvasModule()
  : CanvasModule ("RecoPulseSimpleCanvas")
{
}

RecoPulseSimpleCanvasModule::~RecoPulseSimpleCanvasModule()
{

}

Bool_t RecoPulseSimpleCanvasModule::Initialize()
{
  m_c = new TCanvas("c_RecoPulseSimple", "c_RecoPulseSimple", 640, 640);
  m_c->Divide(2,2);
  AddCanvas(m_c);
  return true;
}

Bool_t RecoPulseSimpleCanvasModule::BeginRun()
{
  //StoredObject<RawData::Event> ev;
  return true;
}

Bool_t RecoPulseSimpleCanvasModule::ProcessEvent()
{
  TDirectory* dir = gDirectory;
  TH1* h_low = (TH1*)dir->FindObjectAny("h_nHits_low");
  TH1* h_high = (TH1*)dir->FindObjectAny("h_nHits_high");
  if (h_low && h_high) {
    m_c->cd(1);
    h_low->Draw("hist");
    h_high->Draw("histsame");
  }
  h_low = (TH1*)dir->FindObjectAny("h_totalQ_low");
  h_high = (TH1*)dir->FindObjectAny("h_totalQ_high");
  if (h_low && h_high) {
    m_c->cd(2);
    h_low->Draw("hist");
    h_high->Draw("histsame");
  }
  h_low = (TH1*)dir->FindObjectAny("h_Qratio_low");
  h_high = (TH1*)dir->FindObjectAny("h_Qratio_high");
  if (h_low && h_high) {
    m_c->cd(3);
    h_low->Draw("hist");
    h_high->Draw("histsame");
  }
  h_low = (TH1*)dir->FindObjectAny("h_hitT_low");
  h_high = (TH1*)dir->FindObjectAny("h_hitT_high");
  if (h_low && h_high) {
    m_c->cd(4);
    h_low->Draw("hist");
    h_high->Draw("histsame");
  }
  m_c->Update();
  return true;
}

Bool_t RecoPulseSimpleCanvasModule::EndRun()
{
  return true;
}

Bool_t RecoPulseSimpleCanvasModule::Finalize()
{
  return true;
}

