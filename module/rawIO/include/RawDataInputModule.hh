#ifndef _JSNS2_RawDataInputModule_h
#define _JSNS2_RawDataInputModule_h

#include "Module.hh"
#include "FileDescriptor.hh"
#include "SharedMemory.hh"

namespace JSNS2 {
  
  class RawDataInputModule : public Module {
    
  public:
    RawDataInputModule(const char* name);
    virtual ~RawDataInputModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
    void OpenStatus(const char* path);
    
  private:
    Bool_t Read();
    
  protected:
    FileDescriptor* m_fd;
    
  private:
    UInt_t* m_buf;
    Bool_t m_isFirst;
    SharedMemory m_shm;

  };

}

#endif

