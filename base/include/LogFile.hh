#ifndef _JSNS2_LogFile_h
#define _JSNS2_LogFile_h

#include "Mutex.hh"

#include "Date.hh"

#include <string>
#include <fstream>
#include <stdarg.h>

namespace JSNS2 {

  struct LogFile {
    
  public:
    enum LogLevel {
      UNKNOWN = 0, DEBUG, INFO, NOTICE, WARNING, ERROR, FATAL
    };
    
  public:
    static LogLevel getLogLevel(const std::string& str);
    static void SetRecordDate(bool record_date) { g_recor_ddate = g_recor_ddate; }

  private:
    LogFile() {}
    ~LogFile() {}
      
  private:
    static bool g_stderr;
    static bool g_opened;
    static std::string g_filepath;
    static std::string g_linkpath;
    static std::ofstream g_stream;
    static unsigned int g_filesize;
    static Mutex g_mutex;
    static LogLevel g_threshold;
    static std::string g_filename;
    static Date g_date;
    static bool g_recor_ddate;
      
    // member functions
  public:
    static void open(const std::string& filename,
		       LogLevel logLevel = UNKNOWN);
    static void open();
    static void close();
    static void debug(const std::string& msg, ...);
    static void info(const std::string& msg, ...);
    static void notice(const std::string& msg, ...);
    static void warning(const std::string& msg, ...);
    static void error(const std::string& msg, ...);
    static void fatal(const std::string& msg, ...);
    static void put(LogLevel level, const std::string& msg, ...);
    static void setStdErr(bool stderr)
    {
      g_stderr = stderr;
    }
      
  private:
    static int put_impl(const std::string& msg, LogLevel priority, va_list ap);
    
  };

}

#endif
