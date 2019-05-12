#include "RawDataInputModule.hh"

#include "StoredObject.hh"
#include "EventMetaData.hh"
#include "RawEvent.hh"
#include "RawFADCArray.hh"
#include "RunStatus.hh"

using namespace JSNS2;

RawDataInputModule::RawDataInputModule(const char* name) : Module(name)
{
  m_fd = NULL;
  m_buf = NULL;
}

RawDataInputModule::~RawDataInputModule()
{
  if (m_fd) {
    m_fd->Close();
    delete m_fd;
  }
  if (m_buf) delete [] m_buf;
  m_fd = NULL;
  m_buf= NULL;
}

void RawDataInputModule::OpenStatus(const char* path)
{
  m_shm.Open(path, sizeof(RunStatus));
}

Bool_t RawDataInputModule::Initialize()
{
  m_buf = new UInt_t[1024*1024*5];//20MB                                                                                 
  StoredObject<EventMetaData>::Create();
  StoredObject<RawEvent>::Create();
  StoredObject<RawFADCArray>::Create();
  return true;
}

Bool_t RawDataInputModule::BeginRun()
{
  if (m_shm.GetFd() > 0) {
    RunStatus* status = (RunStatus*)m_shm.Map();
    status->runNum = 0;
    status->state[0] = 0;
    strcpy(status->state, "READY");
    status->eventNum = 0;
    status->nevents = 0;
    status->nbytes = 0;
  }
  m_isFirst = true;
  return Read();
}

Bool_t RawDataInputModule::Read()
{
  StoredObject<RawEvent> ev;
  StoredObject<RawFADCArray> fadcs;
  ev->Reset();
  fadcs->Reset();
  m_fd->Read(&(ev->GetHeader()), sizeof(int));
  m_fd->Read((&ev->GetHeader()+1), (ev->GetHeaderSize() - 1) * sizeof(int));
  for (unsigned int i = 0; i < ev->GetNboards(); i++) {
    m_fd->Read(m_buf, sizeof(int)); // read serial number
    m_fd->Read(m_buf+1, sizeof(int)); // read waveform data
    int nword = m_buf[1] & 0x0FFFFFFF;
    m_fd->Read(m_buf+2, (nword - 1) * sizeof(int));
    fadcs->Add(RawFADC(m_buf));
    ev->Add(RawDataBlock(nword+1, m_buf));
  }
  m_fd->Read(&ev->GetTrailer(), sizeof(int) * ev->GetTrailerSize());
  if (m_shm.GetFd() > 0) {
    RunStatus* status = (RunStatus*)m_shm.Map();
    status->runNum = ev->GetRunNumber();
    status->state[0] = 0;
    strcpy(status->state, "RUNNING");
    status->eventNum = ev->GetEventNumber();
    status->nevents = status->nevents+1;
    status->nbytes = status->nbytes + ev->GetSize() * sizeof(int);
  }
  StoredObject<EventMetaData> meta;
  meta->SetRunNumber(ev->GetRunNumber());
  meta->SetEventNumber(ev->GetEventNumber());
  meta->SetTriggerTime(ev->GetTriggerTime());
  return true;
}

Bool_t RawDataInputModule::ProcessEvent()
{
  if (m_isFirst) {
    m_isFirst = false;
    return true;
  }
  return Read();
}

Bool_t RawDataInputModule::EndRun()
{
  m_isFirst = false;
  return true;
}

Bool_t RawDataInputModule::Finalize()
{
  if (m_fd) {
    m_fd->Close();
    delete m_fd;
  }
  if (m_buf) delete [] m_buf;
  m_fd = NULL;
  m_buf= NULL;
  m_isFirst = false;
  return true;
}


