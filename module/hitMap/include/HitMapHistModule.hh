#ifndef _JSNS2_HitMapHistModule_h
#define _JSNS2_HitMapHistModule_h

#include "HistModule.hh"

#include <TH1.h>

#include <map>

namespace JSNS2 {
  
  class HitMapHistModule : public HistModule {
    
  public:
    HitMapHistModule();
    virtual ~HitMapHistModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:
    
  private:
    TH1* m_PMT_charge_low;
    TH1* m_PMT_timing_low;
    TH1* m_charge_avg_low;
    TH1* m_timing_avg_low;
    TH1* m_PMT_charge_high;
    TH1* m_PMT_timing_high;
    TH1* m_charge_avg_high;
    TH1* m_timing_avg_high;
    
  };

}

#endif

