#ifndef _JSNS2_RootFileOutputModule_h
#define _JSNS2_RootFileOutputModule_h

#include "Module.hh"

#include <TFile.h>
#include <TTree.h>

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
    
  private:
    TFile* m_file;
    TTree* m_tree;
    std::string m_path;
    
  };

}

#endif

