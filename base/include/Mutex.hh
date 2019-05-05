#ifndef __JSNS2_Mutex_hh
#define __JSNS2_Mutex_hh

#include <pthread.h>

namespace JSNS2 {

  class Mutex {
    
    friend class Cond;
    
    // constructors & destructors
  public:
    Mutex();
    ~Mutex();
    
    // member methods
  public:
    bool Lock();
    bool Unlock();
    
    // member data
  private:
    pthread_mutex_t m_mu;
    pthread_mutexattr_t m_attr;
    
  };

}

#endif
