#include "RawEvent.hh"

ClassImp(JSNS2::RawEvent);

using namespace JSNS2;

RawEvent::RawEvent()
{
  memset(&m_header, 0, sizeof(m_header));
  memset(&m_trailer, 0, sizeof(m_trailer));
}

RawEvent::RawEvent(const RawEvent& ev)
{
  memcpy(&m_header, &ev.m_header, sizeof(m_header));
  memcpy(&m_trailer, &ev.m_trailer, sizeof(m_trailer));
  m_blocks = std::vector<RawDataBlock>();
  for (auto& block : ev()) {
    m_blocks.push_back(block);
  }
}

RawEvent::~RawEvent()
{
}

void RawEvent::Reset()
{
  memset(&m_header, 0, sizeof(m_header));
  memset(&m_trailer, 0, sizeof(m_trailer));
  m_blocks = std::vector<RawDataBlock>();
}

const RawEvent& RawEvent::operator=(const RawEvent& ev)
{
  memcpy(&m_header, &ev.m_header, sizeof(m_header));
  memcpy(&m_trailer, &ev.m_trailer, sizeof(m_trailer));
  m_blocks = std::vector<RawDataBlock>();
  for (auto& block : ev()) {
    m_blocks.push_back(block);
  }
  return *this;
}

void RawEvent::SetNboards(UInt_t nboards)
{
  m_header.nboards = nboards;
  if (m_blocks.size() != nboards) {
    m_blocks = std::vector<RawDataBlock>(nboards);
  }
}

UInt_t RawEvent::GetSize() const
{
  UInt_t size = GetHeaderSize() + GetTrailerSize();
  for (auto& block : (*this)()) {
    size +=block.Data().size();
  }
  return size;
}
