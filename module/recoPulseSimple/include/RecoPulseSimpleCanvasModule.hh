#ifndef _JSNS2_RecoPulseSimpleCanvasModule_h
#define _JSNS2_RecoPulseSimpleCanvasModule_h

#include "CanvasModule.hh"

namespace JSNS2 {
  
  class RecoPulseSimpleCanvasModule : public CanvasModule {
    
  public:
    RecoPulseSimpleCanvasModule();
    virtual ~RecoPulseSimpleCanvasModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:
    
  private:
    //TCanvas* m_dT;
    TCanvas* m_c;
    
  };

}

#endif

