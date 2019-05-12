#include "RecoPulseSimpleModule.hh"

#include "RawEvent.hh"
#include "RawFADCArray.hh"
#include "RawPulseArray.hh"
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
  StoredObject<RawPulseArray>::Create();
  return true;
}


Bool_t RecoPulseSimpleModule::BeginRun()
{
  return true;
}

Bool_t RecoPulseSimpleModule::ProcessEvent()
{
  StoredObject<RawFADCArray> fadcs;
  StoredObject<RawPulseArray> pulses;
  pulses->Reset();
  DBObject<DB::FADCMapping> mapping;
  for (auto& fadc : (*fadcs)()) {
    if (!mapping->HasBoard(fadc.GetSerial())) {
      LogFile::error("Unknown FADC serial 0x%x", fadc.GetSerial());
      continue;
    }
    DB::FADCBoard& board(mapping->GetBoard(fadc.GetSerial()));
    for (auto& ic : board.GetChannels()) {
      DB::FADCChannel& ch(ic.second);
      RawPulse pulse(ch.GetPMT(), ch.GetGain());
      std::vector<UChar_t>& samples(fadc[ch.GetId()]);
      for (size_t i = 0; i < samples.size(); i++) {
	if (i < 25) pulse.SetPedestal(pulse.GetPedestal()+samples[i]);
	else if (i == 25) pulse.SetPedestal(pulse.GetPedestal() / 25.);
	else if (i < 100) {
	  pulse.SetCharge(pulse.GetCharge() + pulse.GetPedestal() - samples[i]);
	  if (pulse.GetTime() == 0 && pulse.GetPedestal() - samples[i] > (ch.GetGain()%2 == 0?5:10)) {
	    pulse.SetTime(i * 2);
	  }
	} else {
	  break;
	}
      }
      pulses->Add(pulse);
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


