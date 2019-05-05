#include "RawDataInputModule.hh"

#include "RawEvent.hh"
#include "StoredObject.hh"
#include "EventMetaData.hh"
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
  UInt_t c = 0;
  int ret = m_fd->Read(m_buf+c, sizeof(int));
  c++;
  UInt_t hsize = (0xFFFF & m_buf[0]);
  ret = m_fd->Read(m_buf+c, (hsize - 1) * sizeof(int));
  c += hsize - 1;
  UInt_t nboards = m_buf[2];
  for (unsigned int i = 0; i < nboards; i++) {
    ret = m_fd->Read(m_buf+c, sizeof(int)); // read serial number
    c++;
    ret = m_fd->Read(m_buf+c, sizeof(int)); // read waveform data
    int nword = *(m_buf+c) & 0x0FFFFFFF;
    c++;
    ret = m_fd->Read(m_buf+c, (nword-1) * sizeof(int));
    c += ret / sizeof(int);
  }
  ret = m_fd->Read(m_buf+c, sizeof(int));
  c++;
  StoredObject<RawEvent> ev;
  ev->Read(m_buf);
  if (m_shm.GetFd() > 0) {
    RunStatus* status = (RunStatus*)m_shm.Map();
    status->runNum = ev->GetRunNumber();
    status->state[0] = 0;
    strcpy(status->state, "RUNNING");
    status->eventNum = ev->GetEventNumber();
    status->nevents = status->nevents+1;
    status->nbytes = status->nbytes + c * sizeof(int);
  }
  StoredObject<EventMetaData> meta;
  //meta->SetRunType(ev->GetRunType());
  meta->SetRunNumber(ev->GetRunNumber());
  meta->SetEventNumber(ev->GetEventNumber());
  meta->SetTriggerTime(ev->GetTriggerTime());
  meta->SetTriggerBit(ev->GetTriggerBit());
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


