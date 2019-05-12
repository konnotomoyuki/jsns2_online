#include "RawFADCUnpackerModule.hh"

#include "StoredObject.hh"
#include "RawEvent.hh"
#include "RawFADCArray.hh"

using namespace JSNS2;

RawFADCUnpackerModule::RawFADCUnpackerModule() : Module("RawFADCUnpacker")
{
}

RawFADCUnpackerModule::~RawFADCUnpackerModule()
{
}

Bool_t RawFADCUnpackerModule::Initialize()
{
  StoredObject<RawFADCArray>::Create();
  return true;
}

Bool_t RawFADCUnpackerModule::BeginRun()
{
  return true;
}

Bool_t RawFADCUnpackerModule::Read()
{
  StoredObject<RawEvent> ev;
  StoredObject<RawFADCArray> fadcs;
  fadcs->Reset();
  for (auto& block : (*ev)()) {
    fadcs->Add(RawFADC(block.Ptr()));
  }
  return true;
}

Bool_t RawFADCUnpackerModule::ProcessEvent()
{
  return Read();
}

Bool_t RawFADCUnpackerModule::EndRun()
{
  return true;
}

Bool_t RawFADCUnpackerModule::Finalize()
{
  return true;
}

