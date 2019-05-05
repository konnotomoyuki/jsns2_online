#include "Cond.hh"

#include <pthread.h>
#include <sys/time.h>

using namespace JSNS2;

Cond::Cond()
{
  Init();
}

Cond::Cond(const Cond& cond)
{
  m_cond_t = cond.m_cond_t;
}

bool Cond::Init()
{
  pthread_condattr_t mattr;
  pthread_condattr_init(&mattr);
  pthread_condattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
  if (pthread_cond_init(&m_cond_t, &mattr) != 0) {
    return false;
  }
  pthread_condattr_destroy(&mattr);

  return true;
}

bool Cond::Signal()
{
  if (pthread_cond_signal(&m_cond_t) == 0) return true;
  else return false;
}

bool Cond::Broadcast()
{
  if (pthread_cond_broadcast(&m_cond_t) == 0) return true;
  else return false;
}

bool Cond::Wait(Mutex& mutex)
{
  if (pthread_cond_wait(&m_cond_t, &mutex.m_mu) != 0) {
    return false;
  }
  return true;
}

bool Cond::Wait(Mutex& mutex, const unsigned int sec, const unsigned int msec)
{
  struct timeval now;
  struct timespec timeout;

  gettimeofday(&now, NULL);
  timeout.tv_sec = now.tv_sec + sec;
  timeout.tv_nsec = now.tv_usec * 1000 + msec;
  int stat = 0;
  if ((stat = pthread_cond_timedwait(&m_cond_t, &mutex.m_mu, &timeout)) != 0) {
    return false;
  }
  return true;
}

bool Cond::Destroy()
{
  if (pthread_cond_destroy(&m_cond_t) == 0) return true;
  else return false;
}
