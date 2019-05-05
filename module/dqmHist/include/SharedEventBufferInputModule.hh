#ifndef _JSNS2_SharedEventBufferInputModule_h
#define _JSNS2_SharedEventBufferInputModule_h

#include "Module.hh"
#include "FileDescriptor.hh"
#include "SharedEventBuffer.hh"

namespace JSNS2 {
  
  class SharedEventBufferInputModule : public Module {
  
  public:
    SharedEventBufferInputModule();
    virtual ~SharedEventBufferInputModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:
    void SetPath(const std::string& path) { m_path = path; }
    const std::string& GetPath() const { return m_path; }
    void SetNword(size_t nword) { m_nword = nword; }
    size_t GetNword() const { return m_nword; }
    
  private:
    Bool_t Read();

  private:
    unsigned int* m_buf;
    Bool_t m_isFirst;
    SharedEventBuffer m_shm;
    std::string m_path;
    size_t m_nword;
    
  };

}

#endif

