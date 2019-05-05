#ifndef _JSNS2_Module_h
#define _JSNS2_Module_h

#include <TObject.h>

namespace JSNS2 {
  
  class Processor;
  
  class Module {
    
  public:
    Module(const char* name) {
      m_name = name;
    }
    virtual ~Module() {}
    
    Module() {}
    
  public:
    virtual Bool_t Initialize() { return true; }
    virtual Bool_t BeginRun() { return true; }
    virtual Bool_t ProcessEvent() { return true; }
    virtual Bool_t EndRun() { return true; }
    virtual Bool_t Finalize() { return true; }
    
  public:
    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }
    Processor* GetProcessor() { return m_processor; }
    void SetProcessor(Processor* p) { m_processor = p; }
    
  private:
    std::string m_name;
    Processor* m_processor;
    
  };

}

#endif

