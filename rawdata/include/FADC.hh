#ifndef _JSNS2_FADC_h_
#define _JSNS2_FADC_h_

#include <TObject.h>
#include <vector>

namespace JSNS2 {

  class FADC : public TObject {
    
  public:
    static const int MAX_CHANNELS = 8;
    
  private:
    // Serial ID in register
    UShort_t m_serial;
    // Event size in data header
    UInt_t m_eventSize;
    // Magic word (0x6) in data header
    UChar_t m_magic;
    // Channel mask in data header
    UInt_t m_channelMask;
    // Pattern in data header
    UShort_t m_pattern;
    // Board ID in data header
    UShort_t m_board;
    // Event size in 32 bit word
    UShort_t m_size;
    // Event counter in data header
    UInt_t m_eventCount;
    // Trigger time tag in data header
    UInt_t m_timeTag;
    // event samples in data body
    std::vector<UChar_t> m_samples[MAX_CHANNELS];
    
  public:
    // default constructor
    FADC();
    FADC(const FADC& fadc);
    virtual ~FADC();
    
    UInt_t Read(UInt_t* buf);
    UInt_t Write(UInt_t* buf);
    
    // Serial ID in register
    UShort_t GetSerial() const { return m_serial; }
    void SetSerial(UShort_t serial) { m_serial = serial; }
    // Event size in data header
    UInt_t GetEventSize() const { return m_eventSize; }
    void SetEventSize(UInt_t eventSize) { m_eventSize = eventSize; }
    // Magic word (0x6) in data header
    UChar_t GetMagic() const { return m_magic; }
    void SetMagic(UChar_t magic) { m_magic = magic; }
    // Channel mask in data header
    UInt_t GetChannelMask() const { return m_channelMask; }
    void SetChannelMask(UInt_t channelMask) { m_channelMask = channelMask; }
    // Pattern in data header
    UShort_t GetPattern() const { return m_pattern; }
    void SetPattern(UShort_t pattern) { m_pattern = pattern; }
    // Board ID in data header
    UShort_t GetBoard() const { return m_board; }
    void SetBoard(UShort_t board) { m_board = board; }
    // Trigger time tag in data header
    UInt_t GetTimeTag() const { return m_timeTag; }
    void SetTimeTag(UInt_t timeTag) { m_timeTag = timeTag; }
    // Event counter in data header
    UInt_t GetEventCount() const { return m_eventCount; }
    void SetEventCount(UInt_t eventCount) { m_eventCount = eventCount; }
    // Waveform samples in data body
    std::vector<UChar_t>& GetSamples(Int_t channel) { return m_samples[channel]; }
    const std::vector<UChar_t>& GetSamples(Int_t channel) const { return m_samples[channel]; }
    
    ClassDef(FADC, 1); 
    
  };

}

#endif
