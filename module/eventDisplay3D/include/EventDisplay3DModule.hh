#ifndef _JSNS2_EventDisplay3DModule_h
#define _JSNS2_EventDisplay3DModule_h

#include "Module.hh"
#include "IDDisplay3D.hh"

#include <TEveManager.h>
#include <TGeoManager.h>

namespace JSNS2 {
  
  class EventDisplay3DModule : public Module {
    
  public:
    EventDisplay3DModule(TEveManager* eve, TGeoManager* geo);
    virtual ~EventDisplay3DModule();
    
  public:
    virtual Bool_t Initialize();
    virtual Bool_t BeginRun();
    virtual Bool_t ProcessEvent();
    virtual Bool_t EndRun();
    virtual Bool_t Finalize();
    
  public:
    
  private:
    IDDisplay3D* m_display;
    
  };

}

#endif

