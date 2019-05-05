#ifndef _JSNS2___template__Module_h
#define _JSNS2___template__Module_h

#include "Module.hh"

namespace JSNS2 {
  
  class __template__Module : public Module {
    
  public:
    __template__Module();
    virtual ~__template__Module();
    
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

