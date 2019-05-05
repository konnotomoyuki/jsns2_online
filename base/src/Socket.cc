#include "Socket.hh"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

#include <sstream>
#include <vector>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>

using namespace JSNS2;
using namespace JSNS2;

int Socket::Connect(const std::string& ip, unsigned short port)
{
  m_ip = ip;
  m_port = port;
  return Connect();
}

int Socket::Connect()
{
  if (m_fd > 0) {
    throw (IOException("Socket is working already."));
  }
  sockaddr_in addr;
  memset(&addr, 0, sizeof(sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(m_port);

  if ((m_fd = ::socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    throw (IOException("Failed to create socket"));
  }
  struct hostent* host = NULL;
  host = gethostbyname(m_ip.c_str());
  if (host == NULL) {
    unsigned long ip_address = inet_addr(m_ip.c_str());
    if ((signed long) ip_address < 0) {
      throw (std::exception());
      throw (IOException("Wrong host name or ip"));
    } else {
      host = gethostbyaddr((char*)&ip_address, sizeof(ip_address), AF_INET);
    }
  }
  addr.sin_addr.s_addr = (*(unsigned long*) host->h_addr_list[0]);

  if (::connect(m_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    Close();
    throw (IOException("Failed to connect host %s:%d",
                       m_ip.c_str(), m_port));
  }

  return m_fd;
}

void Socket::SetSendBufferSize(int size)
{
  if (size > 0) {
    if (setsockopt(m_fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) != 0) {
      throw (IOException("failed to SO_SNDBUF: %s\n", strerror(errno)));
    }
  }
}

void Socket::SetRecvBufferSize(int size)
{
  if (size > 0) {
    if (setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) != 0) {
      throw (IOException("error on SO_RCVBUF: %s\n", strerror(errno)));
    }
  }
}

size_t Socket::Write(const void* buf, size_t count)
{
  size_t c = 0;
  int ret;
  while (c < count) {
    errno = 0;
    ret = send(m_fd, ((unsigned char*)buf + c), (count - c), MSG_NOSIGNAL);
    if (ret <= 0) {
      switch (errno) {
        case EINTR: continue;
        case ENETUNREACH:
        case EHOSTUNREACH:
        case ETIMEDOUT:
          usleep(500);
          continue;
        default:
          throw (IOException("Error while writing"));
      }
    }
    c += ret;
  }
  return c;
}

size_t Socket::Read(void* buf, size_t count)
{
  size_t c = 0;
  int ret;
  while (c < count) {
    errno = 0;
    ret = recv(m_fd, ((unsigned char*)buf + c), (count - c), 0);
    if (ret <= 0) {
      switch (errno) {
        case EINTR: continue;
        case EAGAIN: continue;
        default:
          throw (IOException("Error while reading."));
      }
    }
    c += ret;
  }
  return c;
}

void Socket::Print()
{
  sockaddr_in sa;
  memset(&sa, 0, sizeof(sockaddr_in));
  socklen_t sa_len = sizeof(sa);
  if (getsockname(m_fd, (struct sockaddr*)&sa, (socklen_t*)&sa_len) != 0) {
    perror("getsockname");
  }
  printf("Local IP address is: %s\n", inet_ntoa(sa.sin_addr));
  printf("Local port is: %d\n", (int) ntohs(sa.sin_port));
}

const std::string Socket::GetLocalIP()
{
  sockaddr_in sa;
  memset(&sa, 0, sizeof(sockaddr_in));
  socklen_t sa_len = sizeof(sa);
  if (getsockname(m_fd, (struct sockaddr*)&sa, (socklen_t*)&sa_len) != 0) {
    return "";
  }
  return inet_ntoa(sa.sin_addr);
}

int Socket::GetLocalAddress()
{
  sockaddr_in sa;
  memset(&sa, 0, sizeof(sockaddr_in));
  socklen_t sa_len = sizeof(sa);
  if (getsockname(m_fd, (struct sockaddr*)&sa, (socklen_t*)&sa_len) != 0) {
    return 0;
  }
  return sa.sin_addr.s_addr;
}

int Socket::GetLocalPort()
{
  sockaddr_in sa;
  memset(&sa, 0, sizeof(sockaddr_in));
  socklen_t sa_len = sizeof(sa);
  if (getsockname(m_fd, (struct sockaddr*)&sa, (socklen_t*)&sa_len) != 0) {
    return 0;
  }
  return ntohs(sa.sin_port);
}

unsigned int Socket::GetAddress()
{
  struct hostent* host = NULL;
  host = gethostbyname(m_ip.c_str());
  if (host == NULL) {
    unsigned long ip_address = inet_addr(m_ip.c_str());
    if ((signed long) ip_address < 0) {
      throw (std::exception());
      throw (IOException("Wrong host name or ip"));
    } else {
      host = gethostbyaddr((char*)&ip_address, sizeof(ip_address), AF_INET);
    }
  }
  return (*(unsigned long*) host->h_addr_list[0]);
}
