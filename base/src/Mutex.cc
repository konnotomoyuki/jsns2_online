#include "Mutex.hh"

using namespace JSNS2;

Mutex::Mutex()
{
  pthread_mutexattr_init(&m_attr);
  pthread_mutex_init(&m_mu, &m_attr);
}

Mutex::~Mutex()
{
}

bool Mutex::Lock()
{
  if (pthread_mutex_lock(&m_mu) != 0) {
    return false;
  }
  return true;
}

bool Mutex::Unlock()
{
  if (pthread_mutex_unlock(&m_mu) != 0) {
    return false;
  }
  return true;
}
