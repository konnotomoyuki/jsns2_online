#ifndef ServerSocket_h
#define ServerSocket_h

#include "Socket.hh"
#include "FileDescriptor.hh"

namespace JSNS2 {

  class ServerSocket : public FileDescriptor {
    
  public:
    ServerSocket()
      : m_ip(""), m_port(0) {}
    ServerSocket(const std::string& ip, unsigned short port)
      : m_ip(ip), m_port(port) {}
    virtual ~ServerSocket() {}
    
  public:
    void SetIP(const std::string& ip) { m_ip = ip; }
    void SetPort(unsigned short port) { m_port = port; }
    const std::string& GetIP() const { return m_ip; }
    unsigned short GetPort() const { return m_port; }
    int Open(int nqueue = 5);
    int Open(const std::string& ip, 
	     unsigned short port, 
	     int nqueue = 5);
    Socket Accept();
    
  private:
    std::string m_ip;
    unsigned short m_port;
    
  };

}

#endif

