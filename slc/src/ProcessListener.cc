#include "ProcessListener.hh"
#include "ProcessController.hh"

#include "RCCallback.hh"

#include "LogFile.hh"
#include "Process.hh"

using namespace JSNS2;

void ProcessListener::run()
{
  std::string name = m_con->getName();
  if (!m_con->getProcess().wait()) {
    LogFile::fatal("Failed to wait processed process %s",
		   name.c_str());
    return;
  }
  LogFile::debug(name + " : termineted");
}
