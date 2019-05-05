#include "RawEvent.hh"

ClassImp(JSNS2::RawEvent);

using namespace JSNS2;

RawEvent::RawEvent()
  : m_headerSize(0), m_headerMagic(0), m_runNum(0), 
    m_trailerMagic(0), m_trigBit(0), m_trigTime(0),
    m_eventNum(0)
{

}

RawEvent::RawEvent(const RawEvent& ev)
  : m_headerSize(ev.m_headerSize), m_headerMagic(ev.m_headerMagic), 
    m_runNum(ev.m_runNum), m_trailerMagic(ev.m_trailerMagic), 
    m_trigBit(ev.m_trigBit), m_trigTime(ev.m_trigTime),
    m_eventNum(ev.m_eventNum)
{

}

RawEvent::~RawEvent()
{

}

const RawEvent& RawEvent::operator=(const RawEvent& ev)
{
  m_headerSize = ev.m_headerSize;
  m_headerMagic = ev.m_headerMagic;
  m_runNum = ev.m_runNum;
  m_trailerMagic = ev.m_trailerMagic;
  m_trigBit = ev.m_trigBit;
  m_trigTime = ev.m_trigTime;
  m_eventNum = ev.m_eventNum;
  return *this;
}

void RawEvent::SetNboards(UInt_t nboards)
{
  if (m_fadcs.size() != nboards) {
    m_fadcs = std::vector<FADC>(nboards);
  }
}

UInt_t RawEvent::Read(UInt_t* buf)
{
  m_headerSize = 0xFFFF & *buf;
  m_headerMagic = 0xFFFF & (*buf>>16);
  buf++;
  m_runNum = *buf;
  buf++;
  UInt_t nboards = *buf;
  buf++;
  SetNboards(nboards);
  UInt_t count = 3;
  UInt_t eventNum = 0;
  UInt_t trigTime = 0;
  for (size_t i = 0; i < nboards; i++) {
    FADC& fadc(m_fadcs[i]);
    UInt_t c = fadc.Read(buf);
    buf += c;
    count += c;
    if (trigTime == 0 || trigTime > fadc.GetTimeTag())
      trigTime = fadc.GetTimeTag();
    if (eventNum == 0 || eventNum > fadc.GetEventCount())
      eventNum = fadc.GetEventCount();
  }
  SetEventNumber(eventNum);
  SetTriggerTime(trigTime);
  m_trailerMagic = *buf;
  count++;
  return count;
}

UInt_t RawEvent::Write(UInt_t* buf)
{
  *buf = (m_headerSize & 0xFFFF) |
    ((m_headerMagic & 0xFFFF) << 16);
  buf++;
  *buf = m_runNum;
  buf++;
  *buf = m_fadcs.size();
  buf++;
  UInt_t count = 3;
  for (size_t i = 0; i < m_fadcs.size(); i++) {
    FADC& fadc(m_fadcs[i]);
    UInt_t c = fadc.Write(buf);
    buf += c;
    count += c;
  }
  *buf = m_trailerMagic;
  count++;
  return count;
}

