#ifndef _JSNS2_RawPulseArray_h_
#define _JSNS2_RawPulseArray_h_

#include "RawPulse.hh"

#include <TObject.h>
#include <vector>

namespace JSNS2 {

  class RawPulseArray : public TObject {
    
  public:
    // default constructor
    RawPulseArray() {}
    virtual ~RawPulseArray() {}
    
    void Reset() { m_pulses = std::vector<RawPulse>(); }
    void Add(const RawPulse& pulse) { m_pulses.push_back(pulse); }
    RawPulse& At(Int_t id) { return m_pulses[id]; }
    RawPulse& operator[](Int_t id) { return m_pulses[id]; }
    std::vector<RawPulse>& operator()() { return m_pulses; }
    const std::vector<RawPulse>& operator()() const { return m_pulses; }
    
  private:
    std::vector<RawPulse> m_pulses;
    
  public:
    ClassDef(RawPulseArray, 1); 
    
  };

}

#endif
