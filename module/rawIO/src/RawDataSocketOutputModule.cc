#include "RawDataSocketOutputModule.hh"

#include "Socket.hh"
#include "LogFile.hh"

using namespace JSNS2;

RawDataSocketOutputModule::RawDataSocketOutputModule() : RawDataOutputModule("RawDataSocketOutput")
{
}

RawDataSocketOutputModule::~RawDataSocketOutputModule()
{
}

Bool_t RawDataSocketOutputModule::Initialize()
{
  m_ssocket.Open(m_hostname, m_port);
  LogFile::info("accepting connection to (%s:%d", m_hostname.c_str(), m_port);
  m_socket = m_ssocket.Accept();
  LogFile::info("accepted new connection");
  m_fd = new Socket(m_socket);
  return RawDataOutputModule::Initialize();
}

Bool_t RawDataSocketOutputModule::Finalize()
{
  m_ssocket.Close();
  return RawDataOutputModule::Finalize();
}

