#ifndef _JSNS2_Callback_hh
#define _JSNS2_Callback_hh

#include "PVHandler.hh"
#include "PVManager.hh"

namespace JSNS2 {

  class Callback {

  public:
    Callback();
    virtual ~Callback() {}

  public:
    virtual bool perform(const char* req) = 0;
    virtual void timeout() {}

  public:
    void setTimeout(int timeout) { m_timeout = timeout; }
    int getTimeout() const { return m_timeout; }
    void setName(const std::string& name) { m_name = name; }
    const std::string& getName() const { return m_name; }

  public:
    int add(PVHandler* handler);
    void reset();
    void remove(const std::string& name);
    PVHandlerList& getHandlers() { return m_handlers; }
    PVHandler* getHandler(const std::string& name);

    bool get(const std::string& name, int& val);
    bool get(const std::string& name, float& val);
    bool get(const std::string& name, std::string& val);
    bool set(const std::string& name, int val);
    bool set(const std::string& name, float val);
    bool set(const std::string& name, const std::string& val);
    /*
    bool get(const std::string& node, const std::string& name, int& val);
    bool get(const std::string& node, const std::string& name, float& val);
    bool get(const std::string& node, const std::string& name, std::string& val);
    bool set(const std::string& node, const std::string& name, int val);
    bool set(const std::string& node, const std::string& name, float val);
    bool set(const std::string& node, const std::string& name, const std::string& val);
    */

  private:
    PVHandlerList m_handlers;
    int m_timeout;
    std::string m_name;

  public:
    void setPVManager(PVManager* man) { m_man = man; }

  protected:
    PVManager* m_man;

  };

}

#endif
