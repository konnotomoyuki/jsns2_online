#ifndef _JSNS2_RawDataFileInputModule_h
#define _JSNS2_RawDataFileInputModule_h

#include "RawDataInputModule.hh"

namespace JSNS2 {
  
  class RawDataFileInputModule : public RawDataInputModule {
    
  public:
    RawDataFileInputModule();
    virtual ~RawDataFileInputModule();
    
  public:
    virtual Bool_t Initialize();
    void SetPath(const char* path) {
      if (path) m_path = path;
    }
    
  private:
    std::string m_path;
    
  };
  
}

#endif

