#include "RawDataSocketInputModule.hh"

#include "Socket.hh"
#include "LogFile.hh"

using namespace JSNS2;

RawDataSocketInputModule::RawDataSocketInputModule() : RawDataInputModule("RawDataSocketInput")
{
}

RawDataSocketInputModule::~RawDataSocketInputModule()
{
}

Bool_t RawDataSocketInputModule::Initialize()
{
  Socket* file = new Socket();
  while (true) {
    try {
      file->Connect(m_hostname, m_port);
      break;
    } catch (const IOException& e) {
      LogFile::warning(e.What());
    }
    sleep(5);
  }
  LogFile::info("connected to %s:%d", m_hostname.c_str(), m_port);
  m_fd = file;
  return RawDataInputModule::Initialize();
}

