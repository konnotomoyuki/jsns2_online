#include "EventMetaData.hh"

ClassImp(JSNS2::EventMetaData);

using namespace JSNS2;

EventMetaData::EventMetaData()
  : m_runType(), m_runNum(0), 
    m_trigBit(0), m_trigTime(0),
    m_eventNum(0)
{

}

EventMetaData::EventMetaData(const EventMetaData& ev)
{
  SetRunType(ev.GetRunType());
  SetRunNumber(ev.GetRunNumber());
  SetEventNumber(ev.GetEventNumber());
  SetTriggerBit(ev.GetTriggerBit());
  SetTriggerTime(ev.GetTriggerTime());
}

EventMetaData::~EventMetaData()
{

}

const EventMetaData& EventMetaData::operator=(const EventMetaData& ev)
{
  SetRunType(ev.GetRunType());
  SetRunNumber(ev.GetRunNumber());
  SetEventNumber(ev.GetEventNumber());
  SetTriggerBit(ev.GetTriggerBit());
  SetTriggerTime(ev.GetTriggerTime());
  return *this;
}

