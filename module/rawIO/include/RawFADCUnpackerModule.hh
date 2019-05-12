#ifndef _JSNS2_RawFADCUnpackerModule_h
#define _JSNS2_RawFADCUnpackerModule_h

#include "Module.hh"
#include "FileDescriptor.hh"
#include "SharedMemory.hh"

namespace JSNS2 {
  
  class RawFADCUnpackerModule : public Module {
    
  public:
    RawFADCUnpackerModule();
    virtual ~RawFADCUnpackerModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  private:
    Bool_t Read();
    
  };

}

#endif

