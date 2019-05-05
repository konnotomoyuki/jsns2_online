#ifndef __JSNS2_MMutex_h
#define __JSNS2_MMutex_h

#include <pthread.h>

#ifdef __CINT__
#undef __GNUC__
#define _SYS__SELECT_H_
struct pthread_cond_t;
struct pthread_mutex_t;
#endif

namespace JSNS2 {

  class MMutex {
    
    friend class MCond;
      
  public:
    static size_t GetSize() { return sizeof(pthread_mutex_t); }
    
    // constructors and destructor
  public:
    MMutex();
    MMutex(void*);
    MMutex(const MMutex&);
    ~MMutex();
      
    // member functions;
  public:
    bool Init(void*);
    bool Init();
    bool Set(void*);
    bool Lock();
    bool Trylock();
    bool Unlock();
    bool Destroy();
      
  public:
    const MMutex& operator=(const MMutex&);
    
    // data members;
  private:
    pthread_mutex_t* m_mu;
      
  };

}

#endif
