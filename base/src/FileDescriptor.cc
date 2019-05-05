#include "FileDescriptor.hh"

#include <unistd.h>

using namespace JSNS2;

FileDescriptor::FileDescriptor() 
{
  m_fd = -1;
}

FileDescriptor::FileDescriptor(const FileDescriptor& fd) 
{
  m_fd = fd.m_fd;
}

FileDescriptor::FileDescriptor(int fd) 
{
  m_fd = fd;
}

FileDescriptor::~FileDescriptor() 
{

}

bool FileDescriptor::Close() 
{
  if (m_fd > 0) {
    if (::close(m_fd) != 0) {
      return false;
    }
  }
  m_fd = -1;
  return true;
}
