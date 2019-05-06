#include "LogFile.hh"

#include <ConfigFile.hh>
#include <StringUtil.hh>

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdarg>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>

using namespace JSNS2;

bool LogFile::g_stderr = true;
bool LogFile::g_opened = false;
std::string LogFile::g_filepath;
std::string LogFile::g_linkpath;
std::ofstream LogFile::g_stream;
unsigned int LogFile::g_filesize = 0;
Mutex LogFile::g_mutex;
LogFile::LogLevel LogFile::g_threshold;
std::string LogFile::g_filename;
Date LogFile::g_date;
bool LogFile::g_record_date = true;

LogFile::LogLevel LogFile::getLogLevel(const std::string& str)
{
  const std::string s = StringUtil::toupper(str);
  if (s == "DEBUG") {
    return DEBUG;
  } else if (s == "INFO") {
    return INFO;
  } else if (s == "NOTICE") {
    return NOTICE;
  } else if (s == "WARNING") {
    return WARNING;
  } else if (s == "ERROR") {
    return ERROR;
  } else if (s == "FATAL") {
    return FATAL;
  }
  return UNKNOWN;
}

void LogFile::SetRecordDate(bool record_date)
{
  g_record_date = record_date;
}

void LogFile::open(const std::string& filename, LogLevel threshold)
{
  if (!g_opened) {
    ConfigFile config("slowcontrol");
    std::string path = config.get("log.dir");
    //if (path.size() == 0) path = config.get("logfile.dir");
    system(("mkdir -p " + path + "/" + filename).c_str());
    g_filename = filename;
    g_date = Date();
    g_filepath = path + StringUtil::form("/%s/%s.log", filename.c_str(), g_date.toString("%Y.%m.%d"));
    g_linkpath = path + StringUtil::form("/%s/latest.log", filename.c_str());
    //"/latest.log";
    g_threshold = threshold;
    g_opened = true;
    open();
  }
}

void LogFile::open()
{
  if (!g_opened) return;
  struct stat st;
  if (stat(g_filepath.c_str(), &st) == 0) {
    g_filesize = st.st_size;
    g_stream.open(g_filepath.c_str(), std::ios::out | std::ios::app);
  } else {
    g_filesize = 0;
    g_stream.open(g_filepath.c_str(), std::ios::out);
  }
  debug("/* ---------- log file opened ---------- */");
  debug("log file : %s (%d) ", g_filepath.c_str(), g_filesize);
  std::string cmd = "ln -sf " + g_filepath + " "+ g_linkpath;
  system(cmd.c_str());
  debug("sym link : %s", g_linkpath.c_str());
}

void LogFile::close()
{
  if (!g_opened) return;
  g_stream.close();
  g_opened = false;
}

void LogFile::debug(const std::string& msg, ...)
{
  va_list ap;
  va_start(ap, msg);
  put_impl(msg, DEBUG, ap);
  va_end(ap);
}

void LogFile::info(const std::string& msg, ...)
{
  va_list ap;
  va_start(ap, msg);
  put_impl(msg, INFO, ap);
  va_end(ap);
}

void LogFile::notice(const std::string& msg, ...)
{
  va_list ap;
  va_start(ap, msg);
  put_impl(msg, NOTICE, ap);
  va_end(ap);
}

void LogFile::warning(const std::string& msg, ...)
{
  va_list ap;
  va_start(ap, msg);
  put_impl(msg, WARNING, ap);
  va_end(ap);
}

void LogFile::error(const std::string& msg, ...)
{
  va_list ap;
  va_start(ap, msg);
  put_impl(msg, ERROR, ap);
  va_end(ap);
}

void LogFile::fatal(const std::string& msg, ...)
{
  va_list ap;
  va_start(ap, msg);
  put_impl(msg, FATAL, ap);
  va_end(ap);
}


void LogFile::put(LogLevel level, const std::string& msg, ...)
{
  va_list ap;
  va_start(ap, msg);
  put_impl(msg, level, ap);
  va_end(ap);
}

int LogFile::put_impl(const std::string& msg, LogLevel level, va_list ap)
{
  g_mutex.Lock();
  if (g_threshold > level) {
    g_mutex.Unlock();
    return 0;
  }
  Date date;
  if (g_date.getDay() != date.getDay()) {
    g_stream.close();
    open();
  }
  std::stringstream ss;
  if (g_record_date) {
    ss << "[" << date.toString() << "] ";
  }
  std::string color = "\x1b[49m\x1b[39m";
  switch (level) {
    case DEBUG:   color = "\x1b[49m\x1b[39m"; ss << "[DEBUG] "; break;
    case INFO:    color = "\x1b[49m\x1b[32m"; ss << "[INFO] "; break;
    case NOTICE:  color = "\x1b[49m\x1b[34m"; ss << "[NOTICE] "; break;
    case WARNING: color = "\x1b[49m\x1b[35m"; ss << "[WARNING] "; break;
    case ERROR:   color = "\x1b[49m\x1b[31m"; ss << "[ERROR] "; break;
    case FATAL:   color = "\x1b[41m\x1b[37m"; ss << "[FATAL] "; break;
    default:                 ss << "[UNKNOWN] "; break;
  }
  static char* s = new char[1024 * 1024 * 5];
  vsprintf(s, msg.c_str(), ap);
  ss << s << std::endl;
  std::string str = ss.str();
  std::cerr << color << str << "\x1b[49m\x1b[39m";
  if (g_opened) {
    g_stream << str;
    g_stream.flush();
    g_filesize += str.size();
  }
  g_mutex.Unlock();
  return (int) str.size();
}
