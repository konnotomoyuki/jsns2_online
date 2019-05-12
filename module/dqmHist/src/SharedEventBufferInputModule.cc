#include "SharedEventBufferInputModule.hh"

#include "StoredObject.hh"
#include "RawEvent.hh"
#include "RawFADCArray.hh"
#include "EventMetaData.hh"

using namespace JSNS2;

SharedEventBufferInputModule::SharedEventBufferInputModule()
  : Module("SharedEventBufferInput")
{
  m_buf = NULL;
}

SharedEventBufferInputModule::~SharedEventBufferInputModule()
{
  if (m_buf) {
    m_shm.Close();
   delete [] m_buf;
  }
  m_buf = NULL;
}

Bool_t SharedEventBufferInputModule::Initialize()
{
  m_buf = new unsigned int[1024*1024*5];//20MB                                                                                 
  StoredObject<EventMetaData>::Create();
  StoredObject<RawFADCArray>::Create();
  m_shm.Open(m_path, m_nword, true);
  m_shm.Init();
  m_isFirst = true;
  return Read();
}

Bool_t SharedEventBufferInputModule::BeginRun()
{
  m_isFirst = false;
  return true;
}

Bool_t SharedEventBufferInputModule::Read()
{
  UInt_t* p = m_buf;
  m_shm.Read(p);
  RawEvent ev;
  memcpy(&(ev.GetHeader()), p, sizeof(ev.GetHeader()));
  p+= ev.GetHeaderSize();
  StoredObject<RawFADCArray> fadcs;
  fadcs->Reset();
  for (unsigned int i = 0; i < ev.GetNboards(); i++) {
    RawFADC fadc;
    p += fadc.Read(p);
    fadcs->Add(fadc);
  }
  p+= ev.GetTrailerSize();
  StoredObject<EventMetaData> meta;
  meta->SetRunNumber(ev.GetRunNumber());
  meta->SetEventNumber(ev.GetEventNumber());
  meta->SetTriggerTime(ev.GetTriggerTime());
  return true;
}

Bool_t SharedEventBufferInputModule::ProcessEvent()
{
  if (m_isFirst) {
    m_isFirst = false;
    return true;
  }
  return Read();
}

Bool_t SharedEventBufferInputModule::EndRun()
{
  m_isFirst = false;
  return true;
}

Bool_t SharedEventBufferInputModule::Finalize()
{
  if (m_buf) {
    m_shm.Close();
   delete [] m_buf;
  }
  m_buf = NULL;
  m_isFirst = false;
  return true;
}


