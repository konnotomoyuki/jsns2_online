#ifndef _JSNS2_RecoPulseSimpleHistModule_h
#define _JSNS2_RecoPulseSimpleHistModule_h

#include "HistModule.hh"

namespace JSNS2 {
  
  class RecoPulseSimpleHistModule : public HistModule {
    
  public:
    RecoPulseSimpleHistModule();
    virtual ~RecoPulseSimpleHistModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:
    
  private:
    TH1* m_dT;
    TH1* m_nHits_low;
    TH1* m_TotalQ_low;
    TH1* m_Qratio_low;
    TH1* m_hitT_low;
    TH1* m_nHits_high;
    TH1* m_TotalQ_high;
    TH1* m_Qratio_high;
    TH1* m_hitT_high;
    
  };

}

#endif

