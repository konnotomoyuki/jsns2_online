#ifndef _JSNS2_CalcPulseModule_h
#define _JSNS2_CalcPulseModule_h

#include "HistModule.hh"

#include <TH1.h>

#include <map>

namespace JSNS2 {
  
  class CalcPulseModule : public HistModule {
    
  public:
    CalcPulseModule();
    virtual ~CalcPulseModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:
    
  private:
    std::map<int, std::vector<TH1*> > m_hs;
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

