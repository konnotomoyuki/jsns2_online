#include "RootFileOutputModule.hh"

#include "DataStore.hh"
#include "LogFile.hh"

#include <TList.h>

using namespace JSNS2;

RootFileOutputModule::RootFileOutputModule()
  : Module ("RootFileOutput")
{
  m_tree = NULL;
  m_file = NULL;
}

RootFileOutputModule::~RootFileOutputModule()
{
  if (m_tree) delete m_tree;
  if (m_file) delete m_file;
}

Bool_t RootFileOutputModule::Initialize()
{
  m_file = new TFile(m_path.c_str(), "recreate");
  m_tree = new TTree("tree", "JSNS2 Event Tree");
  for (auto& it : DataStore::Instance().GetList()) {
    std::string name = it.first;
    bool excluded = false;
    for (auto& exclude_name : m_exclude_names) {
      if (name.find(exclude_name) != std::string::npos) {
	excluded = true;
	break;
      }
    }
    if (!excluded) {
      m_tree->Branch(it.first.c_str(), &(it.second));
    }
  }  
  return true;
}

Bool_t RootFileOutputModule::BeginRun()
{
  return true;
}

Bool_t RootFileOutputModule::ProcessEvent()
{
  m_tree->Fill();
  return true;
}

Bool_t RootFileOutputModule::EndRun()
{
  return true;
}

Bool_t RootFileOutputModule::Finalize()
{
  TList* list = new TList();
  list->SetName("list");
  for (auto& it : DataStore::Instance().GetList()) {
    std::string name = it.first;
    bool excluded = false;
    for (auto& exclude_name : m_exclude_names) {
      if (name.find(exclude_name) != std::string::npos) {
	excluded = true;
	break;
      }
    }
    if (!excluded) {
      list->Add(it.second);
    }
  }
  LogFile::info("Root tree is saved to %s", m_file->GetName());
  m_file->cd();
  list->Write("list", 1);
  m_tree->Write();
  m_file->Close();
  return true;
}


