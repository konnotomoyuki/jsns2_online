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
    
  private:
    std::map<int, PMTHit> m_hits_low;
    std::map<int, PMTHit> m_hits_high;
    
  public:
    ClassDef(PMTHitSummary, 1); 
    
  };

}

#endif
