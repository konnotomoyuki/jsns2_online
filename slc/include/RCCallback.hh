#ifndef _JSNS2_RCCallback_hh
#define _JSNS2_RCCallback_hh

#include "Callback.hh"

#include <string>

namespace JSNS2 {

  class RCCallback : public Callback {

  public:
    RCCallback();
    virtual ~RCCallback() {}

  public:
    virtual void init() {}
    virtual void boot(const std::string& conf) {}
    virtual void load(const std::string& conf) {}
    virtual void start(const std::string runtype, int runno) {}
    virtual void stop() {}
    virtual void abort() {}
    virtual bool perform(const char* req);

  public:
    void setState(const std::string& state) { m_state = state; }
    const std::string& getState() { return m_state; }
    void setRunNumber(int runno) { m_runno = runno; }
    int getRunNumber() const { return m_runno; }
    void setRunType(const std::string& runtype) { m_runtype = runtype; }
    const std::string& getRunType() const { return m_runtype; }

  private:
    int m_runno;
    std::string m_runtype;
    std::string m_state;

  };

};

#endif
