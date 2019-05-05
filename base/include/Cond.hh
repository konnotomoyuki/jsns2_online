#ifndef _JSNS2_Cond_hh
#define _JSNS2_Cond_hh

#include "Mutex.hh"

namespace JSNS2 {

  class Cond {
    
  public:
    Cond();
    Cond(const Cond& cond);
    ~Cond() {};
    
  public:
    bool Init();
    bool Signal();
    bool Broadcast();
    bool Wait(Mutex& mutex);
    bool Wait(Mutex& mutex, const unsigned int sec,
	      const unsigned int msec = 0);
    bool Destroy();
    
  private:
    pthread_cond_t m_cond_t;
    
  };

}

#endif
