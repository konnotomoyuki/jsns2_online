#ifndef __JSNS2_RawDataSocketOutputModule_h
#define __JSNS2_RawDataSocketOutputModule_h

#include "RawDataOutputModule.hh"
#include "ServerSocket.hh"

namespace JSNS2 {
  
  class RawDataSocketOutputModule : public RawDataOutputModule {
    
  public:
    RawDataSocketOutputModule();
    virtual ~RawDataSocketOutputModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t Finalize();
    void SetHostname(const char* hostname) {
      if (hostname) m_hostname = hostname;
    }
    void SetPort(int port) {
      if (port) m_port = port;
    }

  private:
    std::string m_hostname;
    int m_port;
    Socket m_socket;
    ServerSocket m_ssocket;

  };

}

#endif

