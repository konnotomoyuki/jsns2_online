#ifndef _JSNS2_RawDataOutputModule_h
#define _JSNS2_RawDataOutputModule_h

#include "Module.hh"
#include "FileDescriptor.hh"
#include "SharedMemory.hh"

namespace JSNS2 {
  
  class RawDataOutputModule : public Module {
    
  public:
    RawDataOutputModule(const char* name);
    virtual ~RawDataOutputModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
    UInt_t GetPrescale() const { return m_prescale; }
    void SetPrescale(UInt_t scale) { m_prescale = scale; }
    void OpenStatus(const char* path);
    
  protected:
    FileDescriptor* m_fd;
    
  private:
    UInt_t* m_buf;
    UInt_t m_prescale;
    ULong_t m_count;
    SharedMemory m_shm;

  };

}

#endif

