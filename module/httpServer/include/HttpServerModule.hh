#ifndef _JSNS2_HttpServerModule_h
#define _JSNS2_HttpServerModule_h

#include "Module.hh"

#include <THttpServer.h>
#include <TDirectory.h>

namespace JSNS2 {
  
  class HttpServerModule : public Module {
    
  public:
    HttpServerModule();
    virtual ~HttpServerModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:
    void SetHostname(const char* name) { m_hostname = name; } 
    
  private:
    void findObject(THttpServer* http, TDirectory* cdir);
    
  private:
    THttpServer* m_http;
    std::string m_hostname;
    
  };

}

#endif

