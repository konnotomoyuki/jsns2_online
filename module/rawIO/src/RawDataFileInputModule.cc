#include "RawDataFileInputModule.hh"
#include "File.hh"

using namespace JSNS2;

RawDataFileInputModule::RawDataFileInputModule()
  : RawDataInputModule("RawDataFileInput")
{
}

RawDataFileInputModule::~RawDataFileInputModule()
{
}

Bool_t RawDataFileInputModule::Initialize()
{
  File* file = new File();
  file->Open(m_path, "r");
  m_fd = file;
  return RawDataInputModule::Initialize();
}

