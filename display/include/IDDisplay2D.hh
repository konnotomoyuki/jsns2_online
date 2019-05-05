#ifndef __JSNS2_IDDisplay2D_hh_
#define __JSNS2_IDDisplay2D_hh_

#include "IDDisplay.hh"

#include <TH2Poly.h>
#include <TBox.h>
#include <TEllipse.h>
#include <TText.h>
#include <TPolyLine.h>

namespace JSNS2 {

  class IDDisplay2D : public IDDisplay {
    
  public:
    static const double g_barrelX;
    static const double g_barrelY;
    static const double g_barrelX1;
    static const double g_barrelY1;
      static const double g_barrelX2;
    static const double g_barrelY2;
    static const double g_topX;
    static const double g_topY;
    static const double g_bottomX;
    static const double g_bottomY;

  public:
    IDDisplay2D(const char* name, const char* title, const char* unit);
    virtual ~IDDisplay2D();
    
  public:
    virtual void SetBinContent(Int_t i, Double_t v) { 
      if (v <= 0) m_PMTs->SetBinContent(i+4, -20);
      else m_PMTs->SetBinContent(i+4, v);
    }
    virtual Double_t GetBinContent(Int_t i) { return m_PMTs->GetBinContent(i+4); }
    virtual void Draw(Option_t* opt="colz");
    
  private:
    TH2Poly* m_PMTs;
    TBox* m_barrelBox;
    TEllipse* m_topCircle;
    TEllipse* m_bottomCircle;
    TText* m_title;
    TText* m_unit;
    TEllipse* m_plPMTs[200];
    
  };

}

#endif 


