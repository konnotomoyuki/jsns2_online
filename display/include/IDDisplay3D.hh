#ifndef __JSNS2_IDDisplay3D_hh_
#define __JSNS2_IDDisplay3D_hh_

#include "IDDisplay.hh"

#include <TH1.h>

#include <TEveManager.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>

namespace JSNS2 {

  class IDDisplay3D : public IDDisplay {

  public:
    IDDisplay3D(const char* name, const char* title, const char* unit);
    virtual ~IDDisplay3D();
    
  public:
    void SetEveManager(TEveManager* eve) { m_eve = eve; }
    void SetGeoManager(TGeoManager* geom) { m_geom = geom; }
    void SetBinContent(Int_t i, Double_t v);
    Double_t GetBinContent(Int_t i);
    void Draw(Option_t* opt="colz");
    void Update(TH1* h);
    
  private: 
    TEveManager* m_eve;
    TGeoManager* m_geom;
    TGeoVolume *m_PMTs;
    
  };

}

#endif 


