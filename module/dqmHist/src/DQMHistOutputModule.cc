#include "DQMHistOutputModule.hh"
#include "Processor.hh"
#include "HistModule.hh"
#include "StoredObject.hh"
#include "EventMetaData.hh"

#include <TMemFile.h>
#include <TDirectory.h>
#include <TH1.h>

using namespace JSNS2;

DQMHistOutputModule::DQMHistOutputModule()
  : Module ("DQMHistOutput"), m_shm("DQMHist.shm", BUF_SIZE + 2000)
{
  m_path = "./";
  m_prescale = 10;
  m_shm.Open();
  char* p = (char*)m_shm.Map();
  m_mutex.Init(p);
  p+= +m_mutex.GetSize();
  m_cond.Init(p);
  p+= +m_cond.GetSize();
}

DQMHistOutputModule::~DQMHistOutputModule()
{

}

Bool_t DQMHistOutputModule::Initialize()
{
  return true;
}

Bool_t DQMHistOutputModule::BeginRun()
{
  m_count = 0;
  return true;
}

Bool_t DQMHistOutputModule::ProcessEvent()
{
  if (m_count % m_prescale == 0) {
    Processor* pro = GetProcessor();
    char* buf = new char [BUF_SIZE];
    TMemFile* file = new TMemFile("DQMHist.map", buf, BUF_SIZE, "recreate");
    StoredObject<EventMetaData> meta;
    meta->Write();
    file->cd();
    std::vector<Module*>& mods(pro->GetModules());
    for (auto& mod : mods) {
      HistModule* hmod = dynamic_cast<HistModule*>(mod);
      if (hmod == NULL) continue;
      TDirectory* dir = file->mkdir(hmod->GetName().c_str());
      dir->cd();
      std::vector<TH1*>& hs(hmod->GetHists());
      for (auto& h : hs) {
	h->Write();
      }
      file->cd();
    }
    file->Close();
    char* p = (char*)m_shm.Map();
    p+= +m_mutex.GetSize();
    p+= +m_cond.GetSize();
    m_mutex.Lock();
    file->CopyTo(p, file->GetSize());
    m_mutex.Unlock();
    m_cond.Broadcast();
    delete [] buf;
    delete file;
  }
  m_count++;
  return true;
}

Bool_t DQMHistOutputModule::EndRun()
{
  StoredObject<EventMetaData> meta;
  TFile* file = new TFile(Form("DQM_r%06d.root", meta->GetRunNumber()), "recreate");
  meta->Write();
  Processor* pro = GetProcessor();
  std::vector<Module*>& mods(pro->GetModules());
  for (auto& mod : mods) {
    HistModule* hmod = dynamic_cast<HistModule*>(mod);
    if (hmod == NULL) continue;
    file->cd();
    std::vector<TH1*>& hs(hmod->GetHists());
    for (auto& h : hs) {
      h->Write();
    }
  }
  file->Close();
  return true;
}

Bool_t DQMHistOutputModule::Finalize()
{
  return true;
}


