#include "RawDataOutputModule.hh"

#include "StoredObject.hh"
#include "RawEvent.hh"
#include "RunStatus.hh"

#include <cstdio>

using namespace JSNS2;

RawDataOutputModule::RawDataOutputModule(const char* name) : Module(name)
{
  m_fd = NULL;
  m_buf = NULL;
  m_prescale = 0;
}

RawDataOutputModule::~RawDataOutputModule()
{
  if (m_fd) {
    m_fd->Close();
    delete m_fd;
  }
  if (m_buf) delete [] m_buf;
  m_fd = NULL;
  m_buf= NULL;
}

void RawDataOutputModule::OpenStatus(const char* path)
{
  m_shm.Open(path, sizeof(RunStatus));
}

Bool_t RawDataOutputModule::Initialize()
{
  m_buf = new UInt_t[1024*1024*5];//20MB                                                                                 
  return true;
}

Bool_t RawDataOutputModule::BeginRun()
{
  m_count = 0;
  if (m_shm.GetFd() > 0) {
    RunStatus* status = (RunStatus*)m_shm.Map();
    status->runNum = 0;
    status->state[0] = 0;
    strcpy(status->state, "READY");
    status->eventNum = 0;
    status->nevents = 0;
    status->nbytes = 0;
  }
  return true;
}

Bool_t RawDataOutputModule::ProcessEvent()
{
  StoredObject<RawEvent> ev;
  if (m_prescale == 0 || m_count % m_prescale == 0) {
    UInt_t nword = ev->Write(m_buf);
    m_count++;
    UInt_t c = m_fd->Write(m_buf, nword * sizeof(int)) > 0;
    if (m_shm.GetFd() > 0) {
      RunStatus* status = (RunStatus*)m_shm.Map();
      status->runNum = ev->GetRunNumber();
      status->state[0] = 0;
      strcpy(status->state, "RUNNING");
      status->eventNum = ev->GetEventNumber();
      status->nevents = status->nevents+1;
      status->nbytes = status->nbytes + c * sizeof(int);
    }
  }
  m_count++;
  return true;
}

Bool_t RawDataOutputModule::EndRun()
{
  return true;
}

Bool_t RawDataOutputModule::Finalize()
{
  if (m_fd) {
    m_fd->Close();
    delete m_fd;
  }
  if (m_buf) delete [] m_buf;
  m_buf= NULL;
  return true;
}


