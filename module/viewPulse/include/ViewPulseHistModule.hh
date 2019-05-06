#ifndef _JSNS2_ViewPulseHistModule_h
#define _JSNS2_ViewPulseHistModule_h

#include "HistModule.hh"

#include <TH1.h>

#include <map>

namespace JSNS2 {
  
  class ViewPulseHistModule : public HistModule {
    
  public:
    ViewPulseHistModule();
    virtual ~ViewPulseHistModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:
    
  private:
    std::map<int, std::vector<TH1*> > m_hs;
    
  };

}

#endif

