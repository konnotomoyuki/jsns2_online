#include "MCond.hh"

#include <sys/time.h>

using namespace JSNS2;

MCond::MCond() {}

MCond::MCond(const MCond& cond)
{
  *this = cond;
}

MCond::MCond(void* addr)
{
  Set((pthread_cond_t*)addr);
}

MCond::~MCond() {}

bool MCond::Init(void* addr)
{
  Set(addr);
  Init();
  return true;
}

bool MCond::Init()
{
  pthread_condattr_t attr;
  pthread_condattr_init(&attr);
  pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
  pthread_cond_init(m_cond, &attr);
  pthread_condattr_destroy(&attr);
  return true;
}

bool MCond::Set(void* addr)
{
  m_cond = (pthread_cond_t*)addr;
  return true;
}

bool MCond::Destroy()
{
  pthread_cond_destroy(m_cond);
  return true;
}

bool MCond::Signal()
{
  if (pthread_cond_signal(m_cond) == 0) return true;
  else return false;
}

bool MCond::Broadcast()
{
  if (pthread_cond_broadcast(m_cond) == 0) return true;
  else return false;
}

bool MCond::Wait(MMutex& cond)
{
  if (pthread_cond_wait(m_cond, cond.m_mu) != 0) {
    return false;
  }
  return true;
}

bool MCond::Wait(MMutex& mutex, const unsigned int sec,
                 const unsigned int msec)
{
  struct timeval now;
  struct timespec timeout;

  gettimeofday(&now, NULL);
  timeout.tv_sec = now.tv_sec + sec;
  timeout.tv_nsec = now.tv_usec * 1000 + msec;
  if (pthread_cond_timedwait(m_cond, mutex.m_mu, &timeout) != 0) {
    return false;
  }
  return true;
}

const MCond& MCond::operator=(const MCond& cond)
{
  m_cond = cond.m_cond;
  return *this;
}
