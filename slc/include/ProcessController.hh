#ifndef _JSNS2_ProcessController_h
#define _JSNS2_ProcessController_h

#include "RCCallback.hh"

#include "PThread.hh"
#include "Process.hh"

#include <vector>
#include <string>
#include <sstream>

namespace JSNS2 {

  class ProcessController {

    friend class ProcessSubmitter;

  public:
    ProcessController()
    {
    }
    ~ProcessController()
    {
    }

  public:
    bool init(const std::string& name);
    bool load();
    bool start(int expno, int runno);
    bool stop();
    bool abort();
    const std::string& getName() { return m_name; }
    const std::string& getExecutable() { return m_exename; }
    const Process& getProcess() const { return  m_process; }
    Process& getProcess() { return  m_process; }
    void setName(const std::string& name) { m_name = name; }
    void setExecutable(const std::string& exe) { m_exename = exe; }
    void addArgument(const std::string& arg) { m_arg_v.push_back(arg); }
    void addArgument(const char* format, ...);
    template<typename T>
    void addArgument(T arg);
    void clearArguments() { m_arg_v = std::vector<std::string>(); }
    bool isAlive() { return m_process.isAlive(); }

  private:
    std::string m_name;
    std::string m_exename;
    std::vector<std::string> m_arg_v;
    Process m_process;
    std::string m_message;
    PThread m_th_log;
    PThread m_th_process;
    int m_iopipe[2];

  };

  template<typename T>
  inline void ProcessController::addArgument(T arg)
  {
    std::stringstream ss; ss << arg;
    m_arg_v.push_back(ss.str());
  }

  class ProcessSubmitter {

  public:
    ProcessSubmitter(ProcessController* con, int iopipe[2])
      : m_con(con)
    {
      m_iopipe[0] = iopipe[0];
      m_iopipe[1] = iopipe[1];
    }

  public:
    void run();

  private:
    ProcessController* m_con;
    int m_iopipe[2];

  };

}

#endif
