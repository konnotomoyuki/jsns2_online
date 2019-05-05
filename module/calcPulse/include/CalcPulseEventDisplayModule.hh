#ifndef _JSNS2_CalcPulseEventDisplayModule_h
#define _JSNS2_CalcPulseEventDisplayModule_h

#include "CanvasModule.hh"

#include <IDDisplay2D.hh>

#include <TCanvas.h>

#include <map>

namespace JSNS2 {
  
  class CalcPulseEventDisplayModule : public CanvasModule {
    
  public:
    CalcPulseEventDisplayModule();
    virtual ~CalcPulseEventDisplayModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:

  private:
    TCanvas* m_c;
    IDDisplay2D* m_PMT_charge_low;
    IDDisplay2D* m_PMT_timing_low;
    IDDisplay2D* m_PMT_charge_high;
    IDDisplay2D* m_PMT_timing_high;
  };

}

#endif

