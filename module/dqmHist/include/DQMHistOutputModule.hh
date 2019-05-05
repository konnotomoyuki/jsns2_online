#ifndef _JSNS2_DQMHistOutputModule_h
#define _JSNS2_DQMHistOutputModule_h

#include "Module.hh"

#include <SharedMemory.hh>
#include <MMutex.hh>
#include <MCond.hh>

namespace JSNS2 {
  
  class DQMHistOutputModule : public Module {
    
  public:
    static const unsigned int BUF_SIZE = 20000000;
    
  public:
    DQMHistOutputModule();
    virtual ~DQMHistOutputModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
    void SetPath(const char* path) { m_path = path; }
    void SetPrescale(int prescale) { m_prescale = prescale; }
    
  private:
    SharedMemory m_shm;
    MMutex m_mutex;
    MCond m_cond;
    std::string m_path;
    int m_prescale;
    unsigned long long m_count;
    
  };

}

#endif

