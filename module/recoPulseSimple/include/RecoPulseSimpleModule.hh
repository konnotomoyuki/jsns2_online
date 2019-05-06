#ifndef _JSNS2_RecoPulseSimpleModule_h
#define _JSNS2_RecoPulseSimpleModule_h

#include "Module.hh"

namespace JSNS2 {
  
  class RecoPulseSimpleModule : public Module {
    
  public:
    RecoPulseSimpleModule();
    virtual ~RecoPulseSimpleModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:
    
  private:
    
  };

}

#endif

