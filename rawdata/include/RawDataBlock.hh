#ifndef _JSNS2_RawDataBlock_h_
#define _JSNS2_RawDataBlock_h_

#include <TObject.h>
#include <vector>

namespace JSNS2 {

  class RawDataBlock : public TObject {
    
  private:
    // event samples in data body
    std::vector<UInt_t> m_data;
    
  public:
    // default constructor
    RawDataBlock(int n=0, UInt_t* buf = NULL);
    virtual ~RawDataBlock() {}
    
    // DataBlock samples in data body
    std::vector<UInt_t>& Data() { return m_data; }
    const std::vector<UInt_t>& Data() const { return m_data; }
    UInt_t* Ptr() { return (m_data.size()>0)?&(m_data[0]):NULL; }
    UInt_t GetSize() const { return m_data.size(); }
    
    ClassDef(RawDataBlock, 1); 
    
  };

}

#endif
