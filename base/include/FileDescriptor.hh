#ifndef __JSNS2_FileDescriptor_hh
#define __JSNS2_FileDescriptor_hh

#include <string>

namespace JSNS2 {

  class FileDescriptor {
    
  public:
    FileDescriptor();
    FileDescriptor(const FileDescriptor&);
    FileDescriptor(int fd);
    virtual ~FileDescriptor();
      
  public:
    
    int GetFd() const { return m_fd; }
    virtual size_t Write(const void* v, size_t count) { return 0; }
    virtual size_t Read(void* v, size_t count) { return 0; }
    virtual bool Close();
    
  protected:
    int m_fd;
    
  };

}

#endif
