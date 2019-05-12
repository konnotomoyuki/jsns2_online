#include "RecoPulseSimpleHistModule.hh"

#include "StoredObject.hh"
#include "RawPulseArray.hh"

#include <TH1D.h>

using namespace JSNS2;

RecoPulseSimpleHistModule::RecoPulseSimpleHistModule() : HistModule ("RecoPulseSimpleHist")
{
}

RecoPulseSimpleHistModule::~RecoPulseSimpleHistModule()
{

}

Bool_t RecoPulseSimpleHistModule::Initialize()
{
  m_dT = new TH1D("h_dT", "Trigger time (low); Time difference [us]", 100, 0, 200);
  m_dT->SetLineWidth(2);
  m_dT->SetLineColor(kRed);
  AddHist(m_dT);

  m_nHits_low = new TH1D("h_nHits_low", "# Hit PMTs (low); # Hit PMTs", 100, 0, 200);
  m_TotalQ_low = new TH1D("h_totalQ_low", "total Q (low); Total Q [ADC sum]", 100, 0, 1000000);
  m_Qratio_low = new TH1D("h_Qratio_low", "Q ratio (low); max Q / total Q", 100, 0, 1);
  m_hitT_low = new TH1D("h_hitT_low", "Hit time (low); hit time mean [ns]", 100, 0, 200);
  m_nHits_low->SetLineWidth(2);
  m_nHits_low->SetLineColor(kBlack);
  m_TotalQ_low->SetLineWidth(2);
  m_TotalQ_low->SetLineColor(kBlack);
  m_Qratio_low->SetLineWidth(2);
  m_Qratio_low->SetLineColor(kBlack);
  m_hitT_low->SetLineWidth(2);
  m_hitT_low->SetLineColor(kBlack);
  AddHist(m_nHits_low);
  AddHist(m_TotalQ_low);
  AddHist(m_Qratio_low);
  AddHist(m_hitT_low);

  m_nHits_high = new TH1D("h_nHits_high", "# Hit PMTs (high); # Hit PMTs", 100, 0, 200);
  m_TotalQ_high = new TH1D("h_totalQ_high", "total Q (high); Total Q [ADC sum]", 100, 0, 1000000);
  m_Qratio_high = new TH1D("h_Qratio_high", "Q ratio (high); max Q / total Q", 100, 0, 1);
  m_hitT_high = new TH1D("h_hitT_high", "Hit time (high); hit time mean [ns]", 100, 0, 200);
  m_nHits_high->SetLineWidth(2);
  m_nHits_high->SetLineColor(kRed);
  m_TotalQ_high->SetLineWidth(2);
  m_TotalQ_high->SetLineColor(kRed);
  m_Qratio_high->SetLineWidth(2);
  m_Qratio_high->SetLineColor(kRed);
  m_hitT_high->SetLineColor(kRed);
  m_hitT_high->SetLineWidth(2);
  AddHist(m_nHits_high);
  AddHist(m_TotalQ_high);
  AddHist(m_Qratio_high);
  AddHist(m_hitT_high);

  return true;
}

Bool_t RecoPulseSimpleHistModule::BeginRun()
{
  m_nHits_low->Reset();
  m_TotalQ_low->Reset();
  m_Qratio_low->Reset();
  m_hitT_low->Reset();
  m_nHits_high->Reset();
  m_TotalQ_high->Reset();
  m_Qratio_high->Reset();
  m_hitT_high->Reset();
  return true;
}

Bool_t RecoPulseSimpleHistModule::ProcessEvent()
{
  StoredObject<RawPulseArray> pmts;
  Int_t nHits_low = 0;
  Double_t totalQ_low = 0;
  Double_t maxQ_low = 0;
  Double_t hitT_low = 0;
  Int_t nHits_high = 0;
  Double_t totalQ_high = 0;
  Double_t maxQ_high = 0;
  Double_t hitT_high = 0;
  // fills low gain channels
  for (auto p : (*pmts)()) {
    if (p.GetGain()) {
      if (p.GetTime() > 0) { 
	hitT_high += p.GetTime();
	nHits_high++;
	if (maxQ_high < p.GetCharge()) maxQ_high = p.GetCharge();
	totalQ_high += p.GetCharge();
      }
    } else {
      if (p.GetTime() > 0) { 
	hitT_low += p.GetTime();
	nHits_low++;
	if (maxQ_low < p.GetCharge()) maxQ_low = p.GetCharge();
	totalQ_low += p.GetCharge();
      }
    }
  }
  m_nHits_low->Fill(nHits_low);
  m_TotalQ_low->Fill(totalQ_low);
  m_Qratio_low->Fill(maxQ_low/totalQ_low);
  m_hitT_low->Fill(hitT_low/nHits_low);
  m_nHits_high->Fill(nHits_high);
  m_TotalQ_high->Fill(totalQ_high);
  m_Qratio_high->Fill(maxQ_high/totalQ_high);
  m_hitT_high->Fill(hitT_high/nHits_high);

  return true;
}

Bool_t RecoPulseSimpleHistModule::EndRun()
{
  return true;
}

Bool_t RecoPulseSimpleHistModule::Finalize()
{
  return true;
}
