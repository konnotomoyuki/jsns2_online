#ifndef _JSNS2_RawEvent_h_
#define _JSNS2_RawEvent_h_

#include "FADC.hh"

#include <TObject.h>
#include <vector>

namespace JSNS2 {

  class RawEvent : public TObject {
      
  private:
    // Header word size in header
    UShort_t m_headerSize;
    // Magic word in header
    UShort_t m_headerMagic;
    // Run number in header
    UInt_t m_runNum;
    // Magic word in trailer
    UInt_t m_trailerMagic;
    
    // Trigger bit
    UInt_t m_trigBit;
    // Trigger time clock
    UInt_t m_trigTime;
    // Event number 
    UInt_t m_eventNum;
    
    
    // FADC data in data body
    std::vector<FADC> m_fadcs;
    
  public:
    // default constructor
    RawEvent();
    RawEvent(const RawEvent& ev);
    virtual ~RawEvent();
    
    const RawEvent& operator=(const RawEvent& ev);
    
    UInt_t Read(UInt_t* buf);
    UInt_t Write(UInt_t* buf);
    
    // Magic word in header
    UShort_t GetHeaderSize() const { return m_headerSize; }
    void SetHeaderSize(UShort_t size) { m_headerSize = size; }
    // Magic word in header
    UShort_t GetHeaderMagic() const { return m_headerMagic; }
    void SetHeaderMagic(UShort_t magic) { m_headerMagic = magic; }
    // Run Number in header
    UInt_t GetRunNumber() const { return m_runNum; }
    void SetRunNumber(UInt_t runNum) { m_runNum = runNum; }
    // Number of boards in header
    UInt_t GetNboards() const { return m_fadcs.size(); }
    void SetNboards(UInt_t nboards);
    // Event Number in header
    UInt_t GetEventNumber() const { return m_eventNum; }
    void SetEventNumber(UInt_t eventNum) { m_eventNum = eventNum; }
    // Trigger bit
    UInt_t GetTriggerBit() const { return m_trigBit; }
    void SetTriggerBit(UShort_t trigBit) { m_trigBit = trigBit; }
    // Trigger time clock
    UInt_t GetTriggerTime() const { return m_trigTime; }
    void SetTriggerTime(UInt_t trigTime) { m_trigTime = trigTime; }
    // Magic word in trailer
    UInt_t GetTrailerMagic() const { return m_trailerMagic; }
    void SetTrailerMagic(UInt_t magic) { m_trailerMagic = magic; }
    // FADC data in data body
    const std::vector<FADC>& GetFADCs() const { return m_fadcs; }
    std::vector<FADC>& GetFADCs() { return m_fadcs; }
    
    ClassDef(RawEvent, 1); 
    
  };

}

#endif
