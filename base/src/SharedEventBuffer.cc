#include "SharedEventBuffer.hh"

#include <LogFile.hh>

#include <cstring>
#include <cstdio>
#include <unistd.h>

using namespace JSNS2;

size_t SharedEventBuffer::GetSize()
{
  return m_mutex.GetSize() + m_cond.GetSize() +
         sizeof(Header) + sizeof(int) * (m_nword);
}

bool SharedEventBuffer::Open(const std::string& path,
                             size_t nword, bool recreate)
{
  m_nword = nword;
  m_path = "/" + path;
  LogFile::debug("%s", m_path.c_str());
  if (!m_memory.Open(m_path, GetSize())) {
    perror("shm_open");
    LogFile::fatal("Failed to open %s", m_path.c_str());
    return false;
  }
  char* buf = (char*) m_memory.Map(0, GetSize());
  if (buf == NULL) {
    return false;
  }
  m_mutex = MMutex(buf);
  buf += m_mutex.GetSize();
  m_cond = MCond(buf);
  buf += m_cond.GetSize();
  m_header = (Header*)buf;
  buf += sizeof(Header);
  m_buf = (unsigned int*)buf;
  if (recreate) Init();
  return true;
}

bool SharedEventBuffer::Init()
{
  if (m_buf == NULL) return false;
  m_mutex.Init();
  m_cond.Init();
  memset(m_header, 0, sizeof(Header));
  memset(m_buf, 0, sizeof(int) * m_nword);
  return true;
}

void SharedEventBuffer::Clear()
{
  if (m_buf == NULL) return;
  m_mutex.Lock();
  memset(m_header, 0, sizeof(Header));
  memset(m_buf, 0, sizeof(int) * m_nword);
  m_mutex.Unlock();
}

bool SharedEventBuffer::Close()
{
  m_memory.Close();
  return true;
}

bool SharedEventBuffer::Unlink()
{
  m_memory.Close();
  m_memory.Unlink();
  return true;
}

bool SharedEventBuffer::IsWritable(int nword)
{
  if (m_buf == NULL) return false;
  m_mutex.Lock();
  bool writable = m_header->nword_in - m_header->nword_out < m_nword - (nword + 1);
  m_mutex.Unlock();
  return writable;
}

bool SharedEventBuffer::IsReadable(int nword)
{
  if (m_buf == NULL) return false;
  m_mutex.Lock();
  bool readable = m_header->nword_in - m_header->nword_out >= m_nword - (nword + 1);
  m_mutex.Unlock();
  return readable;

}

size_t SharedEventBuffer::Write(const unsigned int* buf, size_t nword)
{
  if (m_buf == NULL) return 0;
  if (nword == 0) return 0;
  if (nword > m_nword) return -1;
  m_mutex.Lock();
  while (m_header->nreader > 0) {
    m_cond.Wait(m_mutex);
  }
  m_header->nwriter++;
  unsigned int i_w = 0;
  unsigned int i_r = 0;
  while (true) {
    i_w = m_header->nword_in % m_nword;
    i_r = m_header->nword_out % m_nword;
    if (m_header->nword_in - m_header->nword_out < m_nword - (nword + 1)) {
      if (i_w >= i_r) {
        unsigned int count = m_nword - i_w;
        if (nword + 1 < count) {
          m_buf[i_w] = nword;
          memcpy((m_buf + i_w + 1), buf, sizeof(int) * nword);
        } else {
          m_buf[i_w] = nword;
          memcpy((m_buf + i_w + 1), buf, sizeof(int) * count);
          if (nword >= count)
            memcpy(m_buf, buf + count, sizeof(int) * (nword - count));
        }
      } else {
        m_buf[i_w] = nword;
        memcpy((m_buf + i_w + 1), buf, sizeof(int) * nword);
      }
      break;
    }
    m_header->nwriter--;
    m_cond.Wait(m_mutex);
    m_header->nwriter++;
  }
  m_header->nword_in += nword + 1;
  unsigned int count = ++m_header->count_in;
  m_header->nwriter--;
  m_cond.Broadcast();
  m_mutex.Unlock();
  return count;
}

size_t SharedEventBuffer::Read(unsigned int* buf)
{
  if (m_buf == NULL) return 0;
  m_mutex.Lock();
  m_header->nreader++;
  while (m_header->nwriter > 0) {
    m_cond.Wait(m_mutex);
  }
  unsigned int i_w = 0;
  unsigned int i_r = 0;
  unsigned int nword = 0;
  while (true) {
    i_w = m_header->nword_in % m_nword;
    i_r = m_header->nword_out % m_nword;
    nword = m_buf[i_r];
    if (nword > 0) {
      if (m_header->nword_in - m_header->nword_out >= (nword + 1)) {
        if (i_w > i_r) {
          memcpy(buf, (m_buf + i_r + 1), sizeof(int) * nword);
          break;
        } else if (i_w < i_r) {
          if (m_nword - i_r > nword) {
            memcpy(buf, (m_buf + i_r + 1), sizeof(int) * nword);
            break;
          } else {
            unsigned int count = m_nword - i_r;
            memcpy(buf, (m_buf + i_r + 1), sizeof(int) * count);
            if (nword > count) {
              memcpy(buf + count, m_buf, sizeof(int) * (nword - count));
            }
            break;
          }
        }
      }
    }
    m_header->nreader--;
    m_cond.Wait(m_mutex);
    m_header->nreader++;
  }
  m_header->nword_out += nword + 1;
  unsigned int count = ++m_header->count_out;
  m_header->nreader--;
  m_cond.Broadcast();
  m_mutex.Unlock();
  return count;
}
