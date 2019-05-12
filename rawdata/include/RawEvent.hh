#ifndef _JSNS2_RawEvent_h_
#define _JSNS2_RawEvent_h_

#include "RawDataBlock.hh"

#include <TObject.h>
#include <vector>

namespace JSNS2 {

    struct RawEventHeader {
      UInt_t size;
      UInt_t runNum;
      UInt_t trigTime;
      UInt_t eventNum;
      UShort_t nboards;
      UShort_t magic;
    };

    struct RawEventTrailer {
      UInt_t magic;
      UInt_t chksum;
    };
    
  class RawEvent : public TObject {
      
  private:
    struct RawEventHeader m_header;
    struct RawEventTrailer  m_trailer;
    // RawWaveform data in data body
    std::vector<RawDataBlock> m_blocks;
    
  public:
    // default constructor
    RawEvent();
    RawEvent(const RawEvent& ev);
    virtual ~RawEvent();
    
    const RawEvent& operator=(const RawEvent& ev);
    
    RawEventHeader& GetHeader() { return m_header; }
    RawEventTrailer& GetTrailer() { return m_trailer; }

    void Reset();
    UInt_t GetSize() const;
    // Header size in header
    UShort_t GetHeaderSize() const { return m_header.size; }
    void SetHeaderSize(UShort_t size) { m_header.size = size; }
    // Magic word in header
    UShort_t GetHeaderMagic() const { return m_header.magic; }
    void SetHeaderMagic(UShort_t magic) { m_header.magic = magic; }
    // Run Number in header
    UInt_t GetRunNumber() const { return m_header.runNum; }
    void SetRunNumber(UInt_t runNum) { m_header.runNum = runNum; }
    // Number of boards in header
    UInt_t GetNboards() const { return m_header.nboards; }
    void SetNboards(UInt_t nboards);
    // Event Number in header
    UInt_t GetEventNumber() const { return m_header.eventNum; }
    void SetEventNumber(UInt_t eventNum) { m_header.eventNum = eventNum; }
    // Trigger time clock
    UInt_t GetTriggerTime() const { return m_header.trigTime; }
    void SetTriggerTime(UInt_t trigTime) { m_header.trigTime = trigTime; }
    // Magic word in trailer
    UInt_t GetTrailerMagic() const { return m_trailer.magic; }
    void SetTrailerMagic(UInt_t magic) { m_trailer.magic = magic; }
    // Trailer magic
    UInt_t GetTrailerCheckSum() const { return m_trailer.chksum; }
    void SetTrailerCheckSum(UInt_t chksum) { m_trailer.chksum = chksum; }
    UShort_t GetTrailerSize() const { return sizeof(m_trailer) / sizeof (int); }

    // Raw data in data body
    RawDataBlock& At(Int_t id) { return m_blocks[id]; }
    RawDataBlock& operator[](Int_t id) { return m_blocks[id]; }
    const std::vector<RawDataBlock>& operator()() const { return m_blocks; }
    std::vector<RawDataBlock>& operator()() { return m_blocks; }
    void Add(const RawDataBlock& block) { m_blocks.push_back(block); }

    ClassDef(RawEvent, 1); 
    
  };

}

#endif
