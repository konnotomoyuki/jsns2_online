#include "HistOutputModule.hh"
#include "Processor.hh"
#include "HistModule.hh"
#include "StoredObject.hh"
#include "EventMetaData.hh"

#include <TMemFile.h>
#include <TDirectory.h>
#include <TH1.h>

using namespace JSNS2;

HistOutputModule::HistOutputModule()
  : Module ("HistOutput")
{
  m_path = "";
}

HistOutputModule::~HistOutputModule()
{

}

Bool_t HistOutputModule::Initialize()
{
  return true;
}

Bool_t HistOutputModule::BeginRun()
{
  return true;
}

Bool_t HistOutputModule::ProcessEvent()
{
  return true;
}

Bool_t HistOutputModule::EndRun()
{
  if (m_path.size() == 0) return true;
  StoredObject<EventMetaData> meta;
  TFile* file = new TFile(Form(m_path.c_str(), meta->GetRunNumber()), "recreate");
  meta->Write();
  file->cd();
  Processor* pro = GetProcessor();
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
  delete file;
  return true;
}

Bool_t HistOutputModule::Finalize()
{
  return true;
}
