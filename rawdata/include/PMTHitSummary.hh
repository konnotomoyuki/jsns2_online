#ifndef _JSNS2_PMTHitSummary_h_
#define _JSNS2_PMTHitSummary_h_

#include "PMTHit.hh"

#include <TObject.h>
#include <map>

namespace JSNS2 {

  class PMTHitSummary : public TObject {
    
  public:
    // default constructor
    PMTHitSummary();
    virtual ~PMTHitSummary();
    
    void Add(Int_t id, const PMTHit& hit, Bool_t gain = false);
    PMTHit& Get(Int_t id, Bool_t gain = false);
    std::map<int, PMTHit>& GetList(Bool_t gain = false);
    Double_t GetTime(Bool_t gain = false) const
    { 
      return (gain?m_time_high:m_time_low);
    }
    void SetTime(Double_t time, Bool_t gain = false)
    { 
      if (gain) m_time_high = time;
      else m_time_low = time;
    }
    
  private:
    std::map<int, PMTHit> m_hits_low;
    std::map<int, PMTHit> m_hits_high;
    Double_t m_time_low;
    Double_t m_time_high;
    
  public:
    ClassDef(PMTHitSummary, 1); 
    
  };

}

#endif
