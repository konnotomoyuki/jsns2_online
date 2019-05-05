#ifndef _JSNS2_EBCallback_h
#define _JSNS2_EBCallback_h

#include "RCCallback.hh"
#include "ProcessController.hh"
#include "SharedMemory.hh"
#include "Time.hh"

namespace JSNS2 {

  class EBCallback : public RCCallback {

  public:
    EBCallback();
    virtual ~EBCallback();

  public:
    virtual void init();
    virtual void boot(const std::string& conf);
    virtual void load(const std::string& conf);
    virtual void start(const std::string runtype, int runno);
    virtual void stop();
    virtual void abort();
    virtual void timeout();

  private:
    ProcessController m_con;
    SharedMemory m_shm;
    Time m_time;

  };

}

#endif
