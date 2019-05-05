#include "EBCallback.hh"

#include "RCErrorException.hh"

#include <StringUtil.hh>
#include <LogFile.hh>
#include <Time.hh>
#include <RunStatus.hh>

#include <iostream>
#include <cstring>

using namespace JSNS2;

EBCallback::EBCallback()
{
  setTimeout(5);
  //m_con.setCallback(this);
}

EBCallback::~EBCallback()
{
}

void EBCallback::init()
{
  add(new PVHandlerString("runState", "UNKNOWN"));
  add(new PVHandlerInt("runNum", 0));
  add(new PVHandlerInt("eventNum", 0));
  add(new PVHandlerFloat("nevents", 0));
  add(new PVHandlerFloat("nbytes", 0));
  add(new PVHandlerFloat("eventRate", 0));
  add(new PVHandlerFloat("flowRate", 0));
  m_con.init("eb");
  m_shm.Open("/receiver.shm", sizeof(RunStatus));
  memset(m_shm.Map(), 0, sizeof(RunStatus));
}

void EBCallback::boot(const std::string& config)
{
}

void EBCallback::load(const std::string& config)
{
  try {
    m_con.clearArguments();
    m_con.setExecutable(StringUtil::form("%s/bin/receiver", getenv("JSNS2_HOME")));
    /*
    m_con.setExecutable("root");
    m_con.addArgument("-l");
    m_con.addArgument("-q");
    m_con.addArgument("-b");
    m_con.addArgument("%s/macro/receiver.C", getenv("JSNS2_HOME"));
    */
    m_con.load();
  } catch (const std::exception& e) {
    LogFile::warning("error during process execution %s", e.what());
  }
  LogFile::debug("Load done");
}

/*
   start() : Start triggers to RUNNING
   expno   : Experiment number from RC parent
   runno   : Run number from RC parent
*/
void EBCallback::start(const std::string runtype, int runno)
{
  RunStatus* status = (RunStatus*)m_shm.Map();
  status->type[0] = 0;
  strcpy(status->type, runtype.c_str());
  LogFile::debug("run # = %s.%06d", runtype.c_str(), runno);
  LogFile::debug("Start done");
}

/*
   stop() : End run by stopping triggers to READY
*/
void EBCallback::stop()
{
  memset(m_shm.Map(), 0, sizeof(RunStatus));
  LogFile::debug("Stop done");
}

/*
   resume() : Restart triggers back to RUNNING
   subno    : Sub run number for the resumed run
   returns true on success or false on failed
*/

/*
   abort() : Discard all configuration to back to NOTREADY
*/
void EBCallback::abort()
{
  m_con.abort();
  memset(m_shm.Map(), 0, sizeof(RunStatus));
  LogFile::debug("Abort done");
}

/*
   monitor() : Check status at each end of wainting time limit.
*/
void EBCallback::timeout()
{
  std::string state = getState();
  if (state == "RUNNING" || state == "READY" ||
      state == "PAUSED"  || state == "LOADING" ||
      state == "STARTING") {
    if (!m_con.isAlive()) {
      throw (RCErrorException(m_con.getName() + " : crashed"));
    }
  }

  RunStatus* status = (RunStatus*)m_shm.Map();
  float nevents_last = 0;
  float nbytes_last = 0;
  get("nevents", nevents_last);
  get("nbytes", nbytes_last);
  float nevents = status->nevents;
  float nbytes = status->nbytes;
  if (status->state == std::string("RUNNING")) {
    LogFile::debug("runState = %s", status->state);
    LogFile::debug("runNumber = %u", status->runNum);
    LogFile::debug("eventNumber = %u", status->eventNum);
    LogFile::debug("nevents = %f", nevents);
    LogFile::debug("nbytes = %f", nbytes);
    Time t;
    double dt =t.Get() - m_time.Get() ;
    float flowRate = (nbytes - nbytes_last) / dt;
    float eventRate = (nevents - nevents_last) / dt;
    set("eventRate", (float)eventRate);
    set("flowRate", (float)flowRate);
    m_time = t;
  }
  set("runNum", (int)status->runNum);
  set("runState", status->state);
  set("eventNum", (int)status->eventNum);
  set("nevents", (float)nevents);
  set("nbytes", (float)nbytes);
}

extern "C" {
  void* GetEBCallback()
  {
    return new EBCallback();
  }
}
