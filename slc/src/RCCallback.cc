#include "RCCallback.hh"

#include "RCErrorException.hh"

#include <StringUtil.hh>
#include <LogFile.hh>

#include <cstdlib>
#include <map>

using namespace JSNS2;

RCCallback::RCCallback()
{
  m_runno = 0;
  setState("NOTREADY");
}

bool RCCallback::perform(const char* val)
{
  StringList sl = StringUtil::split(val, '&');
  std::map<std::string, std::string> s;
  for (size_t i = 0; i < sl.size(); i++) {
    StringList ssl = StringUtil::split(sl[i], '=');
    if (ssl.size() > 1) {
      s.insert(std::pair<std::string, std::string>(ssl[0], ssl[1]));
    } else {
      s.insert(std::pair<std::string, std::string>(ssl[0], ""));
    }
  }
  if (s.find("req") == s.end()) {
    LogFile::warning("Request name is not found. Skip request...");
    return false;
  }
  std::string name = StringUtil::toupper(s["req"]);
  std::string state = getState();
  printf("write request (%s) state=%s\n", name.c_str(), state.c_str());
  if (name.find("BOOT") != std::string::npos) {
    if (state == "NOTREADY") {
      m_man->putRCState("BOOTING");
      try {
	boot((s.find("conf")!=s.end()?s["conf"]:""));
	m_man->putRCState("NOTREADY");
      } catch (const RCErrorException& e) {
	LogFile::error("Error during BOOT : %s", e.What());
	m_man->putRCState("ERROR");
      }
      return true;
    } else {
      LogFile::warning("RC state is not NOTREADY (%s)", state.c_str());
    }
  } else if (name.find("LOAD") != std::string::npos) {
    if (state == "NOTREADY") {
      m_man->putRCState("LOADING");
      try {
	load((s.find("conf")!=s.end()?s["conf"]:""));
	m_man->putRCState("READY");
      } catch (const RCErrorException& e) {
	LogFile::error("Error during LOAD : %s", e.What());
	m_man->putRCState("ERROR");
      }
      return true;
    } else {
      LogFile::warning("RC state is not NOTREADY (%s)", state.c_str());
    }
  } else if (name.find("START") != std::string::npos) {
    if (state == "READY") {
      m_man->putRCState("STARTING");
      try {
	start((s.find("runtype")!=s.end()?s["runtype"]:""), 
	      atoi((s.find("runno")!=s.end()?s["runno"].c_str():"")));
	m_man->putRCState("RUNNING"); 
      } catch (const RCErrorException& e) {
	LogFile::error("Error during START : %s", e.What());
	m_man->putRCState("ERROR");
      }
      return true;
    } else {
      LogFile::warning("RC state is not READY (%s)", state.c_str());
    }
  } else if (name.find("STOP") != std::string::npos) {
    if (state == "RUNNING") {
      m_man->putRCState("STOPPING");
      try {
	stop();
	m_man->putRCState("READY");
      } catch (const RCErrorException& e) {
	LogFile::error("Error during STOP : %s", e.What());
	m_man->putRCState("ERROR");
      }
      return true;
    } else {
      LogFile::warning("RC state is RUNNING (%s)", state.c_str());
    }
  } else if (name.find("ABORT") != std::string::npos) {
    m_man->putRCState("ABORTING");
    try {
      abort();
      m_man->putRCState("NOTREADY");
    } catch (const RCErrorException& e) {
	LogFile::error("Error during ABORT : %s", e.What());
      m_man->putRCState("ERROR");
    }
    return true;
  }
  return false;
}
