#ifndef _JSNS2_SharedMemory_h
#define _JSNS2_SharedMemory_h

#include "FileDescriptor.hh"

#include <string>

namespace JSNS2 {

  class SharedMemory : public FileDescriptor {
    
  public:
    static bool Unlink(const std::string& path);
    
    // constructors and destructor
  public:
    SharedMemory();
    SharedMemory(const std::string& path, size_t size);
    SharedMemory(const SharedMemory&);
    virtual ~SharedMemory();
    
    // member functions;
  public:
    virtual bool Open(const std::string& path, size_t size);
    virtual bool Open();
    virtual bool Close();
    bool Unlink();
    bool SeekTo(size_t);
    bool SeekBy(size_t);
    void* Map(size_t, size_t);
    void* Map();
    bool Truncate(size_t size);
    const std::string& GetPath() const { return m_path; }
    size_t GetSize() const { return m_size; }
    
    // operators
  public:
    const SharedMemory& operator=(const SharedMemory&);
    
    // data members;
  private:
    std::string m_path;
    size_t m_size;
    void* m_addr;
      
  };
    
}

#endif
