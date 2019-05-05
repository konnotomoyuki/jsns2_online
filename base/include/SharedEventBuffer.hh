#ifndef __JSNS2_SharedEventBuffer_hh
#define __JSNS2_SharedEventBuffer_hh

#include "SharedMemory.hh"
#include "MMutex.hh"
#include "MCond.hh"

namespace JSNS2 {

  class SharedEventBuffer {
    
  public:
    struct Header {
      unsigned int count_in;
      unsigned int count_out;
      unsigned long long nword_in;
      unsigned long long nword_out;
      unsigned short nwriter;
      unsigned short nreader;
    };
    
  public:
    SharedEventBuffer()
    {
      m_buf = NULL;
      m_nword = 0;
    }
    ~SharedEventBuffer()
    {
      if (m_buf != NULL) {
	m_memory.Close();
      }
    }
    
  public:
    bool Open(const std::string& path,
	      size_t nword, bool recreate = false);
    bool Init();
    bool Close();
    bool Unlink();
    void Clear();
    
  public:
    size_t GetSize();
    const std::string& GetPath() const { return m_path; }
    Header* GetHeader() { return m_header; }
    unsigned int* GetBuffer() { return m_buf; }
    bool IsWritable(int nword);
    bool IsReadable(int nword);
    size_t Write(const unsigned int* buf, size_t nword);
    size_t Read(unsigned int* buf);
    
  private:
    std::string m_path;
    SharedMemory m_memory;
    MMutex m_mutex;
    MCond m_cond;
    Header* m_header;
    unsigned int* m_buf;
    unsigned int m_nword;
    
  };
  
}

#endif
