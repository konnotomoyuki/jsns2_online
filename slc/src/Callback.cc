#include "Callback.hh"

#include <StringUtil.hh>

using namespace JSNS2;

Callback::Callback()
{
  m_man = NULL;
  m_timeout = 2;
}

bool Callback::get(const std::string& name, int& val)
{
  PVHandler* handler = getHandler(name);
  if (handler && handler->getType() == PVHandler::INT) {
    val = ((PVHandlerInt*)handler)->get();
    return true;
  }
  return false;
}

bool Callback::set(const std::string& name, int val)
{
  PVHandler* handler = getHandler(name);
  if (handler && handler->getType() == PVHandler::INT) {
    ((PVHandlerInt*)handler)->set(val);
    if (m_man)m_man->putPV(name.c_str(), val);
    return true;
  }
  return false;
}

bool Callback::get(const std::string& name, float& val)
{
  PVHandler* handler = getHandler(name);
  if (handler && handler->getType() == PVHandler::FLOAT) {
    val = ((PVHandlerFloat*)handler)->get();
    return true;
  }
  return false;
}

bool Callback::set(const std::string& name, float val)
{
  PVHandler* handler = getHandler(name);
  if (handler && handler->getType() == PVHandler::FLOAT) {
    ((PVHandlerFloat*)handler)->set(val);
    if (m_man)m_man->putPV(name.c_str(), val);
    return true;
  }
  return false;
}

bool Callback::get(const std::string& name, std::string& val)
{
  PVHandler* handler = getHandler(name);
  if (handler && handler->getType() == PVHandler::STRING) {
    val = ((PVHandlerString*)handler)->get();
    return true;
  }
  return false;
}

bool Callback::set(const std::string& name, const std::string& val)
{
  PVHandler* handler = getHandler(name);
  if (handler && handler->getType() == PVHandler::STRING) {
    ((PVHandlerString*)handler)->set(val);
    if (m_man)m_man->putPV(name.c_str(), val.c_str());
    return true;
  }
  return false;
}

void Callback::reset()
{
  for (PVHandlerList::iterator it = m_handlers.begin();
       it != m_handlers.end(); it++) {
    if (it->second) delete it->second;
  }
  m_handlers = PVHandlerList();
}

int Callback::add(PVHandler* handler)
{
  if (handler) {
    std::string name = handler->getName();
    if (m_handlers.find(name) != m_handlers.end()) {
      PVHandler* handler_old = m_handlers.find(name)->second;
      delete handler_old;
      m_handlers.erase(name);
    }
    m_handlers.insert(std::pair<std::string, PVHandler*>(name, handler));
    return m_handlers.size();
  }
  return 0;
}

void Callback::remove(const std::string& name)
{
  if (m_handlers.find(name) != m_handlers.end()) {
    PVHandler* handler_old = m_handlers.find(name)->second;
    delete handler_old;
    m_handlers.erase(name);
    return;
  }
}

PVHandler* Callback::getHandler(const std::string& name)
{
  std::string vname = StringUtil::replace(name, getName() + ":", "");
  if (m_handlers.find(vname) != m_handlers.end()) {
    return m_handlers.find(vname)->second;
  }
  if (m_handlers.find(name) != m_handlers.end()) {
    return m_handlers.find(name)->second;
  }
  return NULL;
}

