#include "MMutex.hh"

using namespace JSNS2;

MMutex::MMutex() {}

MMutex::MMutex(const MMutex& mutex)
{
  *this = mutex;
}

MMutex::MMutex(void* addr)
{
  Set((pthread_mutex_t*)addr);
}

MMutex::~MMutex() {}

bool MMutex::Init(void* addr)
{
  Set(addr);
  Init();
  return true;
}

bool MMutex::Init()
{
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(m_mu, &attr);
  pthread_mutexattr_destroy(&attr);
  return true;
}

bool MMutex::Set(void* addr)
{
  m_mu = (pthread_mutex_t*)addr;
  return true;
}

bool MMutex::Lock()
{
  if (pthread_mutex_lock(m_mu) != 0) {
    return false;
  } else {
    return true;
  }
}

bool MMutex::Trylock()
{
  if (pthread_mutex_lock(m_mu) != 0) {
    return false;
  }
  return true;
}

bool MMutex::Unlock()
{
  if (pthread_mutex_unlock(m_mu) != 0) {
    return true;
  } else {
    return false;
  }
}

bool MMutex::Destroy()
{
  if (pthread_mutex_destroy(m_mu) != 0) {
    return true;
  } else {
    return false;
  }
}

const MMutex& MMutex::operator=(const MMutex& mutex)
{
  m_mu = mutex.m_mu;
  return *this;
}
