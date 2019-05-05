#ifndef _JSNS2_DQMCanvasInputModule_h
#define _JSNS2_DQMCanvasInputModule_h

#include "Module.hh"

#include <SharedMemory.hh>
#include <MMutex.hh>
#include <MCond.hh>

#include <TMemFile.h>
#include <TDirectory.h>

namespace JSNS2 {
  
  class DQMCanvasInputModule : public Module {
  
  public:
    static const unsigned int BUF_SIZE = 20000000;
    
  public:
    DQMCanvasInputModule();
    virtual ~DQMCanvasInputModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
    void SetPath(const char* path) { m_path = path; }
    void SetPrescale(int prescale) { m_prescale = prescale; }
    
  private:
    void FindMetaData(TDirectory* cdir);
    
  private:
    SharedMemory m_shm;
    MMutex m_mutex;
    MCond m_cond;
    TMemFile* m_file;
    std::string m_path;
    int m_prescale;
    unsigned long long m_count;
    char* m_buf;
    
  };

}

#endif

