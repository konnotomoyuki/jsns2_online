#include "Executor.hh"
#include "LogFile.hh"

#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <unistd.h>

using namespace JSNS2;

void Executor::setExecutable(const char* format, ...)
{
  va_list ap;
  static char ss[1024];
  va_start(ap, format);
  vsprintf(ss, format, ap);
  va_end(ap);
  m_path = ss;
}

void Executor::setExecutable(const std::string& path)
{
  m_path = path;
}

void Executor::addArg(const char* format, ...)
{
  va_list ap;
  static char ss[1024];
  va_start(ap, format);
  vsprintf(ss, format, ap);
  va_end(ap);
  m_arg_v.push_back(ss);
}

void Executor::addArg(const std::string& arg)
{
  m_arg_v.push_back(arg);
}

bool Executor::execute(bool isdaemon)
{
  char* argv[256];
  argv[0] = (char*)m_path.c_str();
  std::cerr << "" << argv[0] << " ";
  for (size_t i = 0; i < m_arg_v.size(); i++) {
    argv[i + 1] = (char*)m_arg_v[i].c_str();
    std::cerr << "" << argv[i + 1] << " ";
  }
  std::cerr << std::endl;
  argv[m_arg_v.size() + 1] = NULL;
  if (isdaemon) daemon(0, 0);
  if (execvp(m_path.c_str(), argv) == -1) {
    LogFile::error("Faield to start %s", argv[0]);
    return false;
  }

  return true;
}
