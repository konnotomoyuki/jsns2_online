#ifndef __JSNS2_File_h
#define __JSNS2_File_h

#include "FileDescriptor.hh"

#include "IOException.hh"

namespace JSNS2 {

  class File : public FileDescriptor {
      
  public:
    static bool Exist(const std::string& filename);
    
  public:
    File() {}
    File(int fd) : FileDescriptor(fd) {}
    File(const std::string& path,
	 const std::string& mode = "r")
    {
      Open(path, mode);
    }
    virtual ~File() {}
      
  public:
    void Open(const std::string& path,
	      const std::string& mode = "r");
    void Unlink(const std::string& path);
    virtual size_t Write(const void* v, size_t count);
    virtual size_t Read(void* v, size_t count);
    virtual void Sync();

  };

}

#endif
