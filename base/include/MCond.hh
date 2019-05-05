#ifndef _JSNS2_MCond_h
#define _JSNS2_MCond_h

#include "MMutex.hh"

#include <pthread.h>

namespace JSNS2 {

  class MCond {
      
  public:
    static size_t GetSize() { return sizeof(pthread_cond_t); }
    
    // constructors and destructor
  public:
    MCond();
    MCond(const MCond&);
    MCond(void*);
    ~MCond();
    
    // member functions;
  public:
    bool Init(void*);
    bool Init();
    bool Set(void*);
    bool Signal();
    bool Broadcast();
    bool Wait(MMutex& mutex);
    bool Wait(MMutex& mutex, const unsigned int sec,
	      const unsigned int msec = 0);
    bool Destroy();
    
  public:
    const MCond& operator=(const MCond&);
    
    // data members;
  private:
    pthread_cond_t* m_cond;
    
  };

}

#endif
