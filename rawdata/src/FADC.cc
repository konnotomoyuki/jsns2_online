#include "FADC.hh"

#include <cstring>

ClassImp(JSNS2::FADC);

using namespace JSNS2;

FADC::FADC()
  : m_serial(0), m_eventSize(0), m_magic(0), 
    m_channelMask(0), m_pattern(0), m_board(0), 
    m_eventCount(0), m_timeTag(0)
{

}

FADC::FADC(const FADC& fadc)
{
  m_serial = fadc.m_serial;
  m_eventSize = fadc.m_eventSize;
  m_magic = fadc.m_magic;
  m_channelMask = fadc.m_channelMask;
  m_pattern = fadc.m_pattern;
  m_board = fadc.m_board;
  m_size = fadc.m_size;
  m_eventCount = fadc.m_eventCount;
  m_timeTag = fadc.m_timeTag;
  for (int i = 0; i < MAX_CHANNELS; i++)
    m_samples[i] = fadc.m_samples[i];
}

FADC::~FADC()
{
}

UInt_t FADC::Read(UInt_t* buf)
{
  if (buf != NULL) {
    m_serial = *buf;
    buf++;
    m_eventSize = *buf & 0x0FFFFFFF;
    m_magic = (*buf >> 28) & 0xF;
    buf++;
    m_channelMask = *buf & 0xFF;
    m_pattern = (*buf>>8) & 0xFFFF;
    m_board = (*buf>>27) & 0x1F;
    buf++;
    m_eventCount = *buf & 0x00FFFFFF;
    buf++;
    m_timeTag = *buf;
    buf++;
    Int_t nch = 0;
    for (int i = 0; i < MAX_CHANNELS; i++) {
      if ((m_channelMask>>i) & 0x1) nch++;
    }
    UInt_t nsamples = (m_eventSize - 4) * 4 / nch;
    UChar_t* samples = (UChar_t*)buf;
    for (int i = 0; i < MAX_CHANNELS; i++) {
      if (m_samples[i].size() != nsamples) {
	m_samples[i] = std::vector<UChar_t>(nsamples);
      }
      if ((m_channelMask>>i) & 0x1) {
	memcpy(&(m_samples[i][0]), samples, m_samples[i].size());
	samples += m_samples[i].size();
      }
    }
    return m_eventSize + 1;
  } else {
    m_serial = 0;
    m_eventSize = 0;
    m_magic = 0;
    m_channelMask = 0;
    m_pattern = 0;
    m_board = 0;
    m_eventCount = 0;
    m_timeTag = 0;
  }
  return 0;
}

UInt_t FADC::Write(UInt_t* buf)
{
  if (buf != NULL) {
    *buf = m_serial;
    buf++;
    *buf = (0x0FFFFFFF & m_eventSize) | ((m_magic & 0xF) << 28);
    buf++;
    *buf = (m_channelMask & 0xFF) |
      ((m_pattern & 0xFFFF) << 8) |
      ((m_board & 0x1F)<< 27);
    buf++;
    *buf = m_eventCount & 0x00FFFFFF;
    buf++;
    *buf = m_timeTag;
    buf++;
    UChar_t* samples = (UChar_t*) buf;
    for (int i = 0; i < MAX_CHANNELS; i++) {
      if ((m_channelMask>>i) & 0x1) {
	memcpy(samples, &(m_samples[i][0]), m_samples[i].size());
	samples += m_samples[i].size();
      }
    }
    return m_eventSize + 1;
  }
  return 0;
}

