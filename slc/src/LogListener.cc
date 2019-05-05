#include "LogListener.hh"

#include "ProcessController.hh"

#include <File.hh>

#include <StringUtil.hh>
#include <LogFile.hh>

#include <iostream>
#include <sstream>
#include <cerrno>
#include <stdio.h>
#include <ctype.h>

using namespace JSNS2;

void LogListener::run()
{
  File file(m_pipe[0]);
  char c;
  std::stringstream ss;
  std::string s;
  LogFile::LogLevel loglevel = LogFile::UNKNOWN;
  int count = 0;
  try {
    while (true) {
      file.Read(&c, 1);
      if (c != '\n' && iscntrl(c)) continue;
      if (c == '\n' && count > 0) {
        s = m_con->getName() + " : " + ss.str();
        ss.str("");
        if (loglevel == LogFile::UNKNOWN) {
          loglevel = LogFile::DEBUG;
        }
	if (loglevel > LogFile::DEBUG) {
	  LogFile::put(loglevel, s);
	} else {
	  LogFile::debug(s);
	}
        count = 0;
        loglevel = LogFile::UNKNOWN;
      } else if (isprint(c)) {
        if (count == 0 && c == '[') {
          ss << c;
          while (true) {
	    file.Read(&c, 1);
            if (c == ']') {
              ss << c;
              s = ss.str();
              if (s == "[DEBUG]") loglevel = LogFile::DEBUG;
              else if (s == "[INFO]") loglevel = LogFile::INFO;
              else if (s == "[NOTICE]") loglevel = LogFile::NOTICE;
              else if (s == "[WARNING]") loglevel = LogFile::WARNING;
              else if (s == "[ERROR]") loglevel = LogFile::ERROR;
              else if (s == "[FATAL]") loglevel = LogFile::FATAL;
              if (loglevel > 0) {
                count = 0;
                ss.str("");
              } else {
                loglevel = LogFile::DEBUG;
              }
              break;
            }
            ss << c;
          }
        } else {
          if (c != ' ' || count > 0) {
            ss << c;
            count++;
          }
        }
      }
    }
  } catch (const IOException& e) {
    LogFile::debug(e.What());
  }
  close(m_pipe[0]);
}
