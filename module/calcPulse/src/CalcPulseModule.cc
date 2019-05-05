#include "CalcPulseModule.hh"

#include "DBObject.hh"
#include "FADCMapping.hh"

#include "StoredObject.hh"
#include "RawEvent.hh"
#include "PMTHitSummary.hh"

#include "LogFile.hh"

#include <TFile.h>
#include <TH1D.h>

using namespace JSNS2;

CalcPulseModule::CalcPulseModule() : HistModule ("CalcPulse")
{
}

CalcPulseModule::~CalcPulseModule()
{

}

Bool_t CalcPulseModule::Initialize()
{
  DBObject<DB::FADCMapping> mapping("FADCMapping");
  StoredObject<PMTHitSummary>::Create();
  StoredObject<PMTHitSummary> pmts;
  for (auto& ib : mapping->GetBoards()) {
    DB::FADCBoard& board(ib.second);
    std::vector<TH1*> hs;
    for (auto& ic : board.GetChannels()) {
      DB::FADCChannel& ch(ic.second);
      TH1* h = new TH1D(Form("h_pulse_PMT%d_%s", (int)ch.GetPMT(), (ch.GetGain()?"high":"low")),
			Form("PMT #%d;Sampling time [ns]; ADC count", (int)ch.GetPMT()),
			256, 0, 512);
      h->SetLineWidth(2);
      h->SetLineColor((ch.GetGain()?kBlack:kRed));
      hs.push_back(h);
      AddHist(h);
      PMTHit p(ch.GetPMT());
      pmts->Add(ch.GetPMT(), p);
    }
    m_hs.insert(std::pair<int, std::vector<TH1*>>(board.GetSerial(), hs));
  }
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

Bool_t CalcPulseModule::BeginRun()
{
  StoredObject<PMTHitSummary> pmts;
  for (auto& ih : m_hs) {
    std::vector<TH1*>& hs(ih.second);
    for (auto& h : hs) {
      h->Reset();
    }
  }
  m_charge_avg_low->Reset();
  m_timing_avg_low->Reset();
  m_charge_avg_high->Reset();
  m_timing_avg_high->Reset();
  return true;
}

Bool_t CalcPulseModule::ProcessEvent()
{
  StoredObject<RawEvent> ev;
  StoredObject<PMTHitSummary> pmts;
  DBObject<DB::FADCMapping> mapping;
  m_charge_avg_low->Reset();
  m_timing_avg_low->Reset();
  m_charge_avg_high->Reset();
  m_timing_avg_high->Reset();
  for (auto& fadc : ev->GetFADCs()) {
    if (!mapping->HasBoard(fadc.GetSerial())) {
      LogFile::error("Unknown FADC serial 0x%x", fadc.GetSerial());
      continue;
    }
    DB::FADCBoard& board(mapping->GetBoard(fadc.GetSerial()));
    std::vector<TH1*>& hs(m_hs[fadc.GetSerial()]);
    for (auto& ic : board.GetChannels()) {
      DB::FADCChannel& ch(ic.second);
      std::vector<UChar_t>& samples(fadc.GetSamples(ch.GetId()));
      TH1* h = hs[ch.GetId()];
      PMTHit& pmt(pmts->Get(ch.GetPMT(), ch.GetGain()));
      PMTHit p(ch.GetPMT());
      for (size_t i = 0; i < samples.size(); i++) {
	h->SetBinContent(i+1, samples[i]);
	if (i < 25) p.SetPedestal(p.GetPedestal()+samples[i]);
	else if (i == 25) p.SetPedestal(p.GetPedestal() / 25.);
	else if (i < 100) {
	  p.SetCharge(p.GetCharge() + p.GetPedestal() - samples[i]);
	  if (p.GetTime() == 0 && p.GetPedestal() - samples[i] > (ch.GetGain()%2 == 0?5:20)) {
	    p.SetTime(i * 2);
	  }
	}
      }
      pmt = p;
      if (!ch.GetGain()) {
	m_PMT_charge_low->SetBinContent(p.GetId()+1, p.GetCharge());
	m_PMT_timing_low->SetBinContent(p.GetId()+1, p.GetTime());
	m_charge_avg_low->Fill(p.GetCharge());
	m_timing_avg_low->Fill(p.GetTime());
      } else {
	m_PMT_charge_high->SetBinContent(p.GetId()+1, p.GetCharge());
	m_PMT_timing_high->SetBinContent(p.GetId()+1, p.GetTime());
	m_charge_avg_high->Fill(p.GetCharge());
	m_timing_avg_high->Fill(p.GetTime());
      }
    }
  }
  return true;
}

Bool_t CalcPulseModule::EndRun()
{
  return true;
}

Bool_t CalcPulseModule::Finalize()
{
  return true;
}


