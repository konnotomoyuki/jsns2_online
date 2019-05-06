#include "RecoPulseSimpleModule.hh"

#include "RawEvent.hh"
#include "PMTHitSummary.hh"
#include "StoredObject.hh"
#include "DBObject.hh"
#include "FADCMapping.hh"
#include "LogFile.hh"

using namespace JSNS2;

RecoPulseSimpleModule::RecoPulseSimpleModule() : Module ("RecoPulseSimple")
{

}

RecoPulseSimpleModule::~RecoPulseSimpleModule()
{

}

Bool_t RecoPulseSimpleModule::Initialize()
{
  DBObject<DB::FADCMapping> mapping("FADCMapping");
  StoredObject<PMTHitSummary>::Create();
  StoredObject<PMTHitSummary> pmts;
  for (auto& ib : mapping->GetBoards()) {
    DB::FADCBoard& board(ib.second);
    for (auto& ic : board.GetChannels()) {
      DB::FADCChannel& ch(ic.second);
      PMTHit p(ch.GetPMT());
      pmts->Add(ch.GetPMT(), p);
    }
  }

  return true;
}


Bool_t RecoPulseSimpleModule::BeginRun()
{
  return true;
}

Bool_t RecoPulseSimpleModule::ProcessEvent()
{
  StoredObject<RawEvent> ev;
  StoredObject<PMTHitSummary> pmts;
  DBObject<DB::FADCMapping> mapping;
  for (auto& fadc : ev->GetFADCs()) {
    if (!mapping->HasBoard(fadc.GetSerial())) {
      LogFile::error("Unknown FADC serial 0x%x", fadc.GetSerial());
      continue;
    }
    DB::FADCBoard& board(mapping->GetBoard(fadc.GetSerial()));
    for (auto& ic : board.GetChannels()) {
      DB::FADCChannel& ch(ic.second);
      PMTHit& pmt(pmts->Get(ch.GetPMT(), ch.GetGain()));
      PMTHit p(ch.GetPMT());
      std::vector<UChar_t>& samples(fadc.GetSamples(ch.GetId()));
      for (size_t i = 0; i < samples.size(); i++) {
	if (i < 25) p.SetPedestal(p.GetPedestal()+samples[i]);
	else if (i == 25) p.SetPedestal(p.GetPedestal() / 25.);
	else if (i < 100) {
	  p.SetCharge(p.GetCharge() + p.GetPedestal() - samples[i]);
	  if (p.GetTime() == 0 && p.GetPedestal() - samples[i] > (ch.GetGain()%2 == 0?5:10)) {
	    p.SetTime(i * 2);
	  }
	} else {
	  break;
	}
      }
      pmt = p;
    }
  }
  return true;
}

Bool_t RecoPulseSimpleModule::EndRun()
{
  return true;
}

Bool_t RecoPulseSimpleModule::Finalize()
{
  return true;
}


