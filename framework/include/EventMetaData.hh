#ifndef _JSNS2_EventMetaData_h_
#define _JSNS2_EventMetaData_h_

#include <TObject.h>
#include <vector>

namespace JSNS2 {

  class EventMetaData : public TObject {
    
  private:
    // Run type
    std::string m_runType;
    // Run number in header
    UInt_t m_runNum;
    // Trigger bit
    UInt_t m_trigBit;
    // Trigger time clock
    UInt_t m_trigTime;
    // EventMetaData number 
    UInt_t m_eventNum;
    
  public:
    // default constructor
    EventMetaData();
    EventMetaData(const EventMetaData& ev);
    virtual ~EventMetaData();
    
  public:
    const EventMetaData& operator=(const EventMetaData& ev);
    
    // Run Type in header
    const std::string& GetRunType() const { return m_runType; }
    void SetRunType(const std::string& runType) { m_runType = runType; }
    void SetRunType(const char* runType) { m_runType = runType; }
    // Run Number in header
    UInt_t GetRunNumber() const { return m_runNum; }
    void SetRunNumber(UInt_t runNum) { m_runNum = runNum; }
    // EventMetaData Number in header
    UInt_t GetEventNumber() const { return m_eventNum; }
    void SetEventNumber(UInt_t eventNum) { m_eventNum = eventNum; }
    // Trigger bit
    UInt_t GetTriggerBit() const { return m_trigBit; }
    void SetTriggerBit(UShort_t trigBit) { m_trigBit = trigBit; }
    // Trigger time clock
    UInt_t GetTriggerTime() const { return m_trigTime; }
    void SetTriggerTime(UInt_t trigTime) { m_trigTime = trigTime; }
    
    ClassDef(EventMetaData, 1); 
    
  };

}

#endif
