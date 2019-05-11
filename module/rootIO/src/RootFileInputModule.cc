#include "RootFileInputModule.hh"

#include "DataStore.hh"

#include <TList.h>

using namespace JSNS2;

RootFileInputModule::RootFileInputModule() : Module ("RootFileInput")
{
  m_tree = NULL;
  m_file = NULL;
}

RootFileInputModule::~RootFileInputModule()
{
  if (m_tree) delete m_tree;
  if (m_file) delete m_file;
}

void RootFileInputModule::Add(TObject* obj)
{
  DataStore::Instance().Add(obj);
}

Bool_t RootFileInputModule::Initialize()
{
  m_file = new TFile(m_path.c_str());
  m_tree = (TTree*)m_file->Get("tree");
  TList* list = (TList*)m_file->Get("list");
  if (list) {
    TIter next(list);
    TObject *obj;
    while ((obj = next())){
      TObject* cobj = obj->Clone();
      Add(cobj);
    }
  }
  for (auto& it : DataStore::Instance().GetList()) {
    m_tree->SetBranchAddress(it.first.c_str(), &(it.second));
  }  
  m_count = 0;
  return true;
}

Bool_t RootFileInputModule::BeginRun()
{
  return true;
}

Bool_t RootFileInputModule::ProcessEvent()
{
  if (m_count >= m_tree->GetEntries()) return false;
  m_tree->GetEntry(m_count);
  m_count++;
  return true;
}

Bool_t RootFileInputModule::EndRun()
{
  return true;
}

Bool_t RootFileInputModule::Finalize()
{
  m_file->cd();
  m_file->Close();
  return true;
}


