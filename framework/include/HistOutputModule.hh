#ifndef _JSNS2_HistOutputModule_h
#define _JSNS2_HistOutputModule_h

#include "Module.hh"

namespace JSNS2 {
  
  class HistOutputModule : public Module {
    
  public:
    HistOutputModule();
    virtual ~HistOutputModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
    void SetPath(const char* path) { m_path = path; }
    
  private:
    std::string m_path;
    
  };

}

#endif

