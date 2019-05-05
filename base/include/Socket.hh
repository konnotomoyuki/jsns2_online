#ifndef __JSNS2_Socket_hh
#define __JSNS2_Socket_hh

#include <string>

#include "FileDescriptor.hh"
#include "IOException.hh"

namespace JSNS2 {
  
  class Socket : public FileDescriptor {
    
    friend class ServerSocket;
    
  public:
    Socket() : m_ip(""), m_port(0) {}
    Socket(const Socket& s) : FileDescriptor(s.m_fd), m_ip(s.m_ip), m_port(s.m_port) {}
    Socket(const std::string& ip, unsigned short port)
      : m_ip(ip), m_port(port) {}
    virtual ~Socket() {}
    
  private:
    Socket(int fd) : FileDescriptor(fd), m_ip(""), m_port(0) {}
    
  public:
    void SetIP(const std::string& ip) { m_ip = ip; }
    void SetPort(unsigned short port) { m_port = port; }
    const std::string& GetIP() const { return m_ip; }
    unsigned short GetPort() const { return m_port; }
    int Connect();
    int Connect(const std::string& ip, unsigned short port);
    void SetSendBufferSize(int size);
    void SetRecvBufferSize(int size);
    void Print();
    const std::string GetLocalIP();
    int GetLocalAddress();
    int GetLocalPort();
    unsigned int GetAddress();
    
  public:
    virtual size_t Write(const void* v, size_t count);
    virtual size_t Read(void* v, size_t count);
    
  private:
    std::string m_ip;
    unsigned short m_port;
    
  };

}

#endif

