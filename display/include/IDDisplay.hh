#ifndef __JSNS2_IDDisplay_hh_
#define __JSNS2_IDDisplay_hh_

#include <TROOT.h>

namespace JSNS2 {

  class IDDisplay {
    
  public:
    static const double g_scale;      // Overall scaling factor
    static const double g_radius;     // Radius of the acrylic tank
    static const double g_height;     // Half of height of the acrylic tank
    static const double g_rPMT;       // radius of the PMT
    
  public:
    IDDisplay() {}
    virtual ~IDDisplay() {}
    
  public:
    int GetNPMTs() const { return m_nPMTs; }
    virtual void SetBinContent(Int_t i, Double_t v) { }
    virtual Double_t GetBinContent(Int_t i) { return 0; }
    virtual void Draw(Option_t* opt="colz") {}
    
  protected:
    int m_nPMTs;

  };

}

#endif 


