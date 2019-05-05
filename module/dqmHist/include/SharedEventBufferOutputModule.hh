#ifndef _JSNS2_SharedEventBufferOutputModule_h
#define _JSNS2_SharedEventBufferOutputModule_h

#include "Module.hh"
#include "SharedEventBuffer.hh"

namespace JSNS2 {
  
  class SharedEventBufferOutputModule : public Module {
    
  public:
    SharedEventBufferOutputModule();
    virtual ~SharedEventBufferOutputModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:
    UInt_t GetPrescale() const { return m_prescale; }
    void SetPrescale(UInt_t scale) { m_prescale = scale; }
    void SetPath(const std::string& path) { m_path = path; }
    const std::string& GetPath() const { return m_path; }
    void SetNword(size_t nword) { m_nword = nword; }
    size_t GetNword() const { return m_nword; }
    
  private:
    unsigned int* m_buf;
    UInt_t m_prescale;
    ULong_t m_count;
    SharedEventBuffer m_shm;
    std::string m_path;
    size_t m_nword;
    
  };

}

#endif

