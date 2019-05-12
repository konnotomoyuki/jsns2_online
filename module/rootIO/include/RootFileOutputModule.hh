#ifndef _JSNS2_RootFileOutputModule_h
#define _JSNS2_RootFileOutputModule_h

#include "Module.hh"

#include <TFile.h>
#include <TTree.h>

#include <vector>
#include <string>

namespace JSNS2 {
  
  class RootFileOutputModule : public Module {
    
  public:
    RootFileOutputModule();
    virtual ~RootFileOutputModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    void SetPath(const char* path) { m_path = path; }
    void Exclude(const std::string& name) { m_exclude_names.push_back(name); }
    
  private:
    TFile* m_file;
    TTree* m_tree;
    std::string m_path;
    UInt_t m_count;
    std::vector<std::string> m_exclude_names;
    
  };

}

#endif

