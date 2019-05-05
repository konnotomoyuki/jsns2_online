#ifndef _JSNS2_RootFileInputModule_h
#define _JSNS2_RootFileInputModule_h

#include "Module.hh"

#include <TFile.h>
#include <TTree.h>

namespace JSNS2 {
  
  class RootFileInputModule : public Module {
    
  public:
    RootFileInputModule();
    virtual ~RootFileInputModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    void SetPath(const char* path) { m_path = path; }
    void Add(TObject* obj);
    
  private:
    TFile* m_file;
    TTree* m_tree;
    std::string m_path;
    UInt_t m_count;
    
  };

}

#endif

