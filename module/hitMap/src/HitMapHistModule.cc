#include "HitMapHistModule.hh"

#include "DBObject.hh"
#include "FADCMapping.hh"

#include "StoredObject.hh"
#include "RawEvent.hh"
#include "PMTHitSummary.hh"

#include "LogFile.hh"

#include <TFile.h>
#include <TH1D.h>

using namespace JSNS2;

HitMapHistModule::HitMapHistModule() : HistModule ("HitMapHist")
{
}

HitMapHistModule::~HitMapHistModule()
{

}

Bool_t HitMapHistModule::Initialize()
{
  DBObject<DB::FADCMapping> mapping("FADCMapping");
  StoredObject<PMTHitSummary> pmts;
  int nPMTs = pmts->GetList().size();
  m_PMT_charge_low = new TH1D("h_PMT_charge_low", "ID PMT Charge; Charge [ADC sum]", nPMTs+1, 0, nPMTs);
  m_PMT_timing_low = new TH1D("h_PMT_timing_low", "ID PMT Timing; Hit time [ADC sum]", nPMTs+1, 0, nPMTs);
  m_charge_avg_low = new TH1D("h_charge_avg_low", "PMT charge per channel; Charge [ADC sum]", 100, 0, 5000);
  m_timing_avg_low = new TH1D("h_timing_avg_low", "PMT timing per channel; Hit time [ns]", 100, 0, 200);
  m_charge_avg_low->SetLineWidth(2);
  m_charge_avg_low->SetLineColor(kRed);
  m_timing_avg_low->SetLineWidth(2);
  m_timing_avg_low->SetLineColor(kRed);
  AddHist(m_PMT_charge_low);
  AddHist(m_PMT_timing_low);
  AddHist(m_charge_avg_low);
  AddHist(m_timing_avg_low);

  m_PMT_charge_high = new TH1D("h_PMT_charge_high", "ID PMT Charge; Charge [ADC sum]", nPMTs+1, 0, nPMTs);
  m_PMT_timing_high = new TH1D("h_PMT_timing_high", "ID PMT Timing; Hit time [ADC sum]", nPMTs+1, 0, nPMTs);
  m_charge_avg_high = new TH1D("h_charge_avg_high", "PMT charge per channel; Charge [ADC sum]", 100, 0, 5000);
  m_timing_avg_high = new TH1D("h_timing_avg_high", "PMT timing per channel; Hit time [ns]", 100, 0, 200);
  m_charge_avg_high->SetLineWidth(2);
  m_charge_avg_high->SetLineColor(kBlack);
  m_timing_avg_high->SetLineWidth(2);
  m_timing_avg_high->SetLineColor(kBlack);
  AddHist(m_PMT_charge_high);
  AddHist(m_PMT_timing_high);
  AddHist(m_charge_avg_high);
  AddHist(m_timing_avg_high);
  return true;
}

Bool_t HitMapHistModule::BeginRun()
{
  m_charge_avg_low->Reset();
  m_timing_avg_low->Reset();
  m_charge_avg_high->Reset();
  m_timing_avg_high->Reset();
  return true;
}

Bool_t HitMapHistModule::ProcessEvent()
{
  StoredObject<PMTHitSummary> pmts;
  DBObject<DB::FADCMapping> mapping;
  m_charge_avg_low->Reset();
  m_timing_avg_low->Reset();
  m_charge_avg_high->Reset();
  m_timing_avg_high->Reset();
  // fills low gain channels
  for (auto ip : pmts->GetList(false)) {
    PMTHit& pmt(ip.second);
    m_PMT_charge_low->SetBinContent(pmt.GetId()+1, pmt.GetCharge());
    m_PMT_timing_low->SetBinContent(pmt.GetId()+1, pmt.GetTime());
    m_charge_avg_low->Fill(pmt.GetCharge());
    m_timing_avg_low->Fill(pmt.GetTime());
  }
  // fills high gain channels
  for (auto ip : pmts->GetList(true)) {
    PMTHit& pmt(ip.second);
    m_PMT_charge_high->SetBinContent(pmt.GetId()+1, pmt.GetCharge());
    m_PMT_timing_high->SetBinContent(pmt.GetId()+1, pmt.GetTime());
    m_charge_avg_high->Fill(pmt.GetCharge());
    m_timing_avg_high->Fill(pmt.GetTime());
  }
  return true;
}

Bool_t HitMapHistModule::EndRun()
{
  return true;
}

Bool_t HitMapHistModule::Finalize()
{
  return true;
}


