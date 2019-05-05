#ifndef _JSNS2_HistModule_h
#define _JSNS2_HistModule_h

#include "Module.hh"

#include <TH1.h>

namespace JSNS2 {
  
  class HistModule : public Module {
    
  public:
    HistModule(const char* name);
    virtual ~HistModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
    TH1* AddHist(TH1* h);
    std::vector<TH1*>& GetHists() { return m_hists; };
    const std::vector<TH1*>& GetHists() const { return m_hists; };
    
  private:
    std::vector<TH1*> m_hists;
    
  };

}

#endif
