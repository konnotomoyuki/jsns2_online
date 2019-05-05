#ifndef _JSNS2_CalcPulseCanvasModule_h
#define _JSNS2_CalcPulseCanvasModule_h

#include "CanvasModule.hh"

#include <TCanvas.h>

#include <map>

namespace JSNS2 {
  
  class CalcPulseCanvasModule : public CanvasModule {
    
  public:
    CalcPulseCanvasModule();
    virtual ~CalcPulseCanvasModule();
    
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

