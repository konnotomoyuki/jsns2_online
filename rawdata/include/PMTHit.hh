#ifndef _JSNS2_PMTHit_h_
#define _JSNS2_PMTHit_h_

#include <TObject.h>

namespace JSNS2 {

  class PMTHit : public TObject {
    
  private:
    Int_t m_id;
    Float_t m_charge;
    Float_t m_time;
    Float_t m_hight;
    Float_t m_width;
    Float_t m_pedestal;
    
  public:
    // default constructor
    PMTHit(Int_t id = -1);
    virtual ~PMTHit();
    PMTHit& operator=(const PMTHit& p);
    
  public:
    Int_t GetId() const { return m_id; }
    void SetId(Int_t id) { m_id = id; }
    Float_t GetCharge() const { return m_charge; }
    void SetCharge(Float_t charge) { m_charge = charge; }
    Float_t GetTime() const { return m_time; }
    void SetTime(Float_t time) { m_time = time; }
    Float_t GetHight() const { return m_hight; }
    void SetHight(Float_t hight) { m_hight = hight; }
    Float_t GetWidth() const { return m_width; }
    void SetWidth(Float_t width) { m_width = width; }
    Float_t GetPedestal() const { return m_pedestal; }
    void SetPedestal(Float_t pedestal) { m_pedestal = pedestal; }
    
    ClassDef(PMTHit, 1); 
    
  };

}

#endif
