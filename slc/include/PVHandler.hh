#ifndef _JSNS2_PVHandler_h
#define _JSNS2_PVHandler_h

#include <string>
#include <vector>
#include <map>

namespace JSNS2 {

  class PVHandler {

    friend class Callback;

  public:
    enum Type {
      NONE = 0,
      INT,
      FLOAT,
      STRING
    };

  public:
    PVHandler(const std::string& name, Type type)
      : m_name(name), m_type(type) {}
    virtual ~PVHandler() {}

  public:
    const std::string& getName() const { return m_name; }
    Type getType() const { return m_type; }
    const char* getTypeLabel() const { 
      switch (m_type) {
      case INT: return "int";
      case FLOAT: return "float";
      case STRING: return "string";
      default:
      case NONE: return "none";
      }
    }

  protected:
    std::string m_name;
    Type m_type;

  };

  typedef std::map<std::string, PVHandler*> PVHandlerList;

  template<typename T, PVHandler::Type type>
  class PVHandlerT : public PVHandler {

  public:
    PVHandlerT(const std::string& name, T val)
      : PVHandler(name, type)
    {
      m_val = val;
    }
    PVHandlerT(const std::string& name)
      : PVHandler(name, type)
    {
    }
    virtual ~PVHandlerT() {}

  public:
    bool handleGet(T& val)
    {
      val = m_val;
      return true;
    }
    bool handleSet(const T& val)
    {
      m_val = val;
      return true;
    }
    T& get() { return m_val; }
    const T& get() const { return m_val; }
    void set(const T& val) { m_val = val; }

  protected:
    T m_val;
  };

  typedef PVHandlerT<int, PVHandler::INT> PVHandlerInt;
  typedef PVHandlerT<float, PVHandler::FLOAT> PVHandlerFloat;
  typedef PVHandlerT<std::string, PVHandler::STRING> PVHandlerString;

}

#endif
