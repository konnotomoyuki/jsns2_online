#include "DQMCanvasOutputModule.hh"

#include "Processor.hh"
#include "CanvasModule.hh"
#include "StoredObject.hh"
#include "EventMetaData.hh"

#include <TMemFile.h>
#include <TCanvas.h>

using namespace JSNS2;

DQMCanvasOutputModule::DQMCanvasOutputModule() 
  : Module ("DQMCanvasOutput"), m_shm("DQMCanvas.shm", BUF_SIZE + 2000)
{
  m_path = "./";
  m_prescale = 1000;
  m_shm.Open();
  char* p = (char*)m_shm.Map();
  m_mutex.Init(p);
  p+= +m_mutex.GetSize();
  m_cond.Init(p);
  p+= +m_cond.GetSize();
}

DQMCanvasOutputModule::~DQMCanvasOutputModule()
{

}

Bool_t DQMCanvasOutputModule::Initialize()
{
  return true;
}

Bool_t DQMCanvasOutputModule::BeginRun()
{
  m_count = 0;
  return true;
}

Bool_t DQMCanvasOutputModule::ProcessEvent()
{
  Processor* pro = GetProcessor();
  char* buf = new char [BUF_SIZE];
  TMemFile* file = new TMemFile("DQMCanvas.map", buf, BUF_SIZE, "recreate");
  StoredObject<EventMetaData> meta;
  meta->Write();
  m_mutex.Lock();
  std::vector<Module*>& mods(pro->GetModules());
  TDirectory* cdir = file;//gDirectory;
  for (auto& mod : mods) {
    CanvasModule* cmod = dynamic_cast<CanvasModule*>(mod);
    if (cmod == NULL) continue;
    std::vector<TCanvas*>& cs(cmod->GetCanvases());
    cdir->cd();
    TDirectory* dir = cdir->mkdir(cmod->GetName().c_str());
    dir->cd();
    for (auto& c : cs) {
      c->Write();
    }
  }
  cdir->cd();
  file->Close();
  char* p = (char*)m_shm.Map();
  p+= +m_mutex.GetSize();
  p+= +m_cond.GetSize();
  //memcpy(p, buf, BUF_SIZE);
  file->CopyTo(p, file->GetSize());
  m_mutex.Unlock();
  m_cond.Broadcast();
  delete [] buf;
  delete file;
  m_count++;
  return true;
}

Bool_t DQMCanvasOutputModule::EndRun()
{
  return true;
}

Bool_t DQMCanvasOutputModule::Finalize()
{
  return true;
}
