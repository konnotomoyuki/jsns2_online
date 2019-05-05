#include "RawDataFileOutputModule.hh"

#include "File.hh"
#include "EventMetaData.hh"
#include "StoredObject.hh"

#include <TROOT.h>

using namespace JSNS2;

RawDataFileOutputModule::RawDataFileOutputModule()
  : RawDataOutputModule("RawDataFileOutput")
{
  m_fd = NULL;
  m_automode = false;
}

RawDataFileOutputModule::~RawDataFileOutputModule()
{
}

Bool_t RawDataFileOutputModule::Initialize()
{
  if (!m_automode) {
    File* file = new File();
    file->Open(m_path, "w");
    m_fd = file;
  }
  return RawDataOutputModule::Initialize();
}

Bool_t RawDataFileOutputModule::BeginRun()
{
  if (m_automode) {
    if (m_fd) {
      m_fd->Close();
      delete m_fd;
    }
    StoredObject<EventMetaData> meta;
    File* file = new File();
    file->Open(Form((m_path+"/r%06d.dat").c_str(), meta->GetRunNumber()), "w");
    m_fd = file;
  }
  return RawDataOutputModule::BeginRun();
}

