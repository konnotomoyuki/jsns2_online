#ifndef _JSNS2_CanvasModule_h
#define _JSNS2_CanvasModule_h

#include "Module.hh"

#include <TCanvas.h>

namespace JSNS2 {
  
  class CanvasModule : public Module {
    
  public:
    CanvasModule(const char* name);
    virtual ~CanvasModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
    TCanvas* AddCanvas(TCanvas* h);
    std::vector<TCanvas*>& GetCanvases() { return m_cs; };
    const std::vector<TCanvas*>& GetCanvases() const { return m_cs; };
    
  private:
    std::vector<TCanvas*> m_cs;

  };

}

#endif
