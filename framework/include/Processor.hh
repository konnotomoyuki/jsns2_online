#ifndef _JSNS2_Processor_h
#define _JSNS2_Processor_h

#include <TObject.h>

#include <vector>

namespace JSNS2 {
  
  class Module;

  class Processor {
  
  public:
    Processor() {}
    virtual ~Processor() {}
      
  public:
    virtual void Run(UInt_t maxEvent = 0);
    void Add(Module* mod);
    std::vector<Module*>& GetModules() { return m_mods; }
    
  private:
    std::vector<Module*> m_mods;
    
  };

}

#endif

