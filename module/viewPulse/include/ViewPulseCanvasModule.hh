#ifndef _JSNS2_ViewPulseCanvasModule_h
#define _JSNS2_ViewPulseCanvasModule_h

#include "CanvasModule.hh"

#include <TCanvas.h>

#include <map>

namespace JSNS2 {
  
  class ViewPulseCanvasModule : public CanvasModule {
    
  public:
    ViewPulseCanvasModule();
    virtual ~ViewPulseCanvasModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:

  private:
    std::map<int, TCanvas*> m_cs;
    
  };

}

#endif

