#ifndef _JSNS2_RawFADCArray_h_
#define _JSNS2_RawFADCArray_h_

#include "RawFADC.hh"

#include <TObject.h>
#include <vector>

namespace JSNS2 {

  class RawFADCArray : public TObject {
    
  public:
    // default constructor
    RawFADCArray() {}
    virtual ~RawFADCArray() {}
    
    void Reset() { m_fadcs = std::vector<RawFADC>(); }
    void Add(const RawFADC& fadc) { m_fadcs.push_back(fadc); }
    RawFADC& At(Int_t id) { return m_fadcs[id]; }
    RawFADC& operator[](Int_t id) { return m_fadcs[id]; }
    std::vector<RawFADC>& operator()() { return m_fadcs; }
    const std::vector<RawFADC>& operator()() const { return m_fadcs; }
    
  private:
    std::vector<RawFADC> m_fadcs;
    
  public:
    ClassDef(RawFADCArray, 1); 
    
  };

}

#endif
