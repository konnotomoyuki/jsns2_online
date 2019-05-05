#include "ProcessController.hh"

#include "ProcessListener.hh"
#include "LogListener.hh"

#include "Executor.hh"
#include "PThread.hh"
#include "LogFile.hh"

#include "StringUtil.hh"

#include <cstdlib>
#include <cstdio>
#include <unistd.h>


using namespace JSNS2;

bool ProcessController::init(const std::string& name)
{
  m_name = name;
  return true;
}

bool ProcessController::load()
{
  m_process.cancel();
  m_process.kill(SIGQUIT);
  if (pipe(m_iopipe) < 0) {
    perror("pipe");
    return false;
  }
  m_process = Process(new ProcessSubmitter(this, m_iopipe));
  m_th_log = PThread(new LogListener(this, m_iopipe));
  m_th_process = PThread(new ProcessListener(this));
  return true;
}

bool ProcessController::start(int expno, int runno)
{
  return true;
}

bool ProcessController::stop()
{
  return true;
}

bool ProcessController::abort()
{
  m_process.kill(SIGINT);
  usleep(100000);
  m_process.kill(SIGQUIT);
  m_process.kill(SIGKILL);
  m_process.wait();
  m_th_log.cancel();
  m_th_process.cancel();
  close(m_iopipe[1]);
  close(m_iopipe[0]);
  return true;
}

void ProcessSubmitter::run()
{
  LogFile::SetRecordDate(false);
  dup2(m_iopipe[1], 1);
  dup2(m_iopipe[1], 2);
  close(m_iopipe[0]);
  Executor executor;
  if (m_con->getExecutable().size() == 0) {
    m_con->setExecutable("root");
  }
  executor.setExecutable(m_con->getExecutable());
  for (size_t i = 0; i < m_con->m_arg_v.size(); i++) {
    executor.addArg(m_con->m_arg_v[i]);
  }
  executor.execute();
}

void ProcessController::addArgument(const char* format, ...)
{
  va_list ap;
  static char ss[1024];
  va_start(ap, format);
  vsprintf(ss, format, ap);
  va_end(ap);
  m_arg_v.push_back(ss);
}
