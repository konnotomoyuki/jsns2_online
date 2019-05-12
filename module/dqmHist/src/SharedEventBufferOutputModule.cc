#include "SharedEventBufferOutputModule.hh"

#include "StoredObject.hh"
#include "RawEvent.hh"

#include <cstdio>
#include <iostream>

using namespace JSNS2;

SharedEventBufferOutputModule::SharedEventBufferOutputModule()
  : Module("SharedEventBufferOutput")
{
  m_buf = NULL;
  m_prescale = 10;
}

SharedEventBufferOutputModule::~SharedEventBufferOutputModule()
{
  if (m_buf) {
    m_shm.Close();
   delete [] m_buf;
  }
  m_buf= NULL;
}

Bool_t SharedEventBufferOutputModule::Initialize()
{
  m_buf = new unsigned int[1024*1024*5];//20MB                                                                                 
  m_shm.Open(m_path, m_nword);
  return true;
}

Bool_t SharedEventBufferOutputModule::BeginRun()
{
  m_count = 0;
  return true;
}

Bool_t SharedEventBufferOutputModule::ProcessEvent()
{
  if (m_prescale == 0 || m_count % m_prescale == 0) {
    StoredObject<RawEvent> ev;
    UInt_t c = 0;
    memcpy(m_buf, &ev->GetHeader(), ev->GetHeaderSize() * sizeof(int));
    c += ev->GetHeaderSize();
    for (auto& block : (*ev)()) {
      memcpy(m_buf+c, block.Ptr(), block.GetSize() * sizeof(int));
      c += block.GetSize();
    }
    memcpy(m_buf+c, &ev->GetTrailer(), ev->GetTrailerSize() * sizeof(int));
    c += ev->GetTrailerSize();
    if (!m_shm.IsWritable(c)) return true;
    m_count++;
    return m_shm.Write(m_buf, c) > 0;
  }
  m_count++;
  return true;
}

Bool_t SharedEventBufferOutputModule::EndRun()
{
  return true;
}

Bool_t SharedEventBufferOutputModule::Finalize()
{
  if (m_buf) {
    m_shm.Close();
   delete [] m_buf;
  }
  m_buf= NULL;
  return true;
}


