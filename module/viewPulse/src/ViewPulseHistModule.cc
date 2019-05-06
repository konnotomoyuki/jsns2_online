#include "ViewPulseHistModule.hh"

#include "DBObject.hh"
#include "FADCMapping.hh"

#include "StoredObject.hh"
#include "RawEvent.hh"
#include "PMTHitSummary.hh"

#include "LogFile.hh"

#include <TFile.h>
#include <TH1D.h>

using namespace JSNS2;

ViewPulseHistModule::ViewPulseHistModule() : HistModule ("ViewPulseHist")
{
}

ViewPulseHistModule::~ViewPulseHistModule()
{

}

Bool_t ViewPulseHistModule::Initialize()
{
  DBObject<DB::FADCMapping> mapping("FADCMapping");
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
    }
    m_hs.insert(std::pair<int, std::vector<TH1*>>(board.GetSerial(), hs));
  }
  return true;
}

Bool_t ViewPulseHistModule::BeginRun()
{
  StoredObject<PMTHitSummary> pmts;
  for (auto& ih : m_hs) {
    std::vector<TH1*>& hs(ih.second);
    for (auto& h : hs) {
      h->Reset();
    }
  }
  return true;
}

Bool_t ViewPulseHistModule::ProcessEvent()
{
  StoredObject<RawEvent> ev;
  DBObject<DB::FADCMapping> mapping;
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
      for (size_t i = 0; i < samples.size(); i++) {
	h->SetBinContent(i+1, samples[i]);
      }
    }
  }
  return true;
}

Bool_t ViewPulseHistModule::EndRun()
{
  return true;
}

Bool_t ViewPulseHistModule::Finalize()
{
  return true;
}


