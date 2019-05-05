#ifndef _JSNS2_RawDataFileOutputModule_h
#define _JSNS2_RawDataFileOutputModule_h

#include "RawDataOutputModule.hh"

namespace JSNS2 {
  
  class RawDataFileOutputModule : public RawDataOutputModule {
    
  public:
    RawDataFileOutputModule();
    virtual ~RawDataFileOutputModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    void SetPath(const char* path) {
      if (path) m_path = path;
    }
    void SetAutoMode(bool automode) {
      m_automode = automode;
    }
    
  private:
    std::string m_path;
    bool m_automode;
    
  };

}

#endif

