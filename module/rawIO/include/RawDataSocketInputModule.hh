#ifndef __JSNS2_RawDataSocketInputModule_h
#define __JSNS2_RawDataSocketInputModule_h

#include "RawDataInputModule.hh"

namespace JSNS2 {
  
  class RawDataSocketInputModule : public RawDataInputModule {
    
  public:
    RawDataSocketInputModule();
    virtual ~RawDataSocketInputModule();
    
  public:
    virtual Bool_t Initialize();
    void SetHostname(const char* hostname) {
      if (hostname) m_hostname = hostname;
    }
    void SetPort(int port) {
      if (port) m_port = port;
    }
    
  private:
    std::string m_hostname;
    int m_port;
    
  };

}

#endif

