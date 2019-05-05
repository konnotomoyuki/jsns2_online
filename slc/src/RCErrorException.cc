#include "RCErrorException.hh"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

using namespace JSNS2;

RCErrorException::RCErrorException(const std::string& format, ...)
{
  va_list ap;
  char ss[1024 * 100];
  va_start(ap, format);
  vsprintf(ss, format.c_str(), ap);
  va_end(ap);
  m_comment = ss;
}

