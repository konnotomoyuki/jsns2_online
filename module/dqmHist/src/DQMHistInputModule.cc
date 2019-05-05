#include "DQMHistInputModule.hh"

#include "Processor.hh"
#include "HistModule.hh"
#include "StoredObject.hh"
#include "EventMetaData.hh"

#include <TList.h>
#include <TKey.h>

using namespace JSNS2;

DQMHistInputModule::DQMHistInputModule()
  : Module ("DQMHistInput"), m_shm("DQMHist.shm", BUF_SIZE + 2000), m_file(NULL)
{
  m_buf = new char [BUF_SIZE];
  m_path = "./";
  m_prescale = 1000;
  m_shm.Open();
  char* p = (char*)m_shm.Map();
  m_mutex.Set(p);
  p+= +m_mutex.GetSize();
  m_cond.Set(p);
  p+= +m_cond.GetSize();
}

DQMHistInputModule::~DQMHistInputModule()
{
  if (m_file) {
    m_file->Close();
    delete m_file;
  }
  delete m_buf;
}

Bool_t DQMHistInputModule::Initialize()
{
  StoredObject<EventMetaData>::Create();
  return true;
}

Bool_t DQMHistInputModule::BeginRun()
{
  m_count = 0;
  return true;
}

void DQMHistInputModule::FindMetaData(TDirectory* cdir)
{
  TList* keys = cdir->GetListOfKeys();
  TIter next(keys);
  TKey* key = NULL;
  while((key = (TKey*)next())) {
    TObject* obj = cdir->FindObjectAny(key->GetName());
    if (obj->IsA()->InheritsFrom("JSNS2::EventMetaData")) {
      EventMetaData* meta = (EventMetaData*)obj;
      StoredObject<EventMetaData> emeta;
      emeta->SetRunType(meta->GetRunType());
      emeta->SetRunNumber(meta->GetRunNumber());
      emeta->SetEventNumber(meta->GetEventNumber());
      emeta->SetTriggerBit(meta->GetTriggerBit());
      emeta->SetTriggerTime(meta->GetTriggerTime());
    } else if (obj->IsA()->InheritsFrom(TDirectory::Class())) {
      TDirectory* dir = (TDirectory*) obj;
      DQMHistInputModule::FindMetaData(dir);
    }
  }
}

Bool_t DQMHistInputModule::ProcessEvent()
{
  if (m_file) {
    m_file->Close();
    delete m_file;
    m_file = NULL;
  }
  m_mutex.Lock();
  m_cond.Wait(m_mutex, 1);
  char* p = (char*)m_shm.Map();
  p+= +m_mutex.GetSize();
  p+= +m_cond.GetSize();
  memcpy(m_buf, p, BUF_SIZE);
  m_mutex.Unlock();
  m_file = new TMemFile("DQMHist.map", m_buf, BUF_SIZE);
  FindMetaData(m_file);
  return true;
}

Bool_t DQMHistInputModule::EndRun()
{
  return true;
}

Bool_t DQMHistInputModule::Finalize()
{
  return true;
}
