#ifndef _JSNS2_DummyEventModule_h
#define _JSNS2_DummyEventModule_h

#include "Module.hh"

#include <TF1.h>
#include <TFile.h>
#include <TTree.h>

namespace JSNS2 {
  
  class DummyEventModule : public Module {
    
  public:
    DummyEventModule();
    virtual ~DummyEventModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();

  public:
    void SetRunNumber(UInt_t runNum) { m_runNum = runNum; }
    UInt_t GetRunNumber() { return m_runNum; }
    void SetFilePath(const char* filepath) { m_filepath = filepath; }

  private:
    UInt_t m_runNum;
    UInt_t m_eventNum;
    TF1* m_f;
    std::string m_filepath;
    TFile* m_file;
    TTree* m_tree;
    ULong64_t m_time;

  };

}

#endif

