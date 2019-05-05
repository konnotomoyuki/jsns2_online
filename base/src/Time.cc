#include "Time.hh"

#include <sys/time.h>

#include <cmath>
#include <cstdlib>
#include <sstream>

using namespace JSNS2;

#define MEGA 1000000

Time::Time()
{
  timeval tv;
  gettimeofday(&tv, 0);
  m_s = tv.tv_sec;
  m_us = tv.tv_usec;
}

Time::Time(const double t)
{
  Set(t);
}

Time::Time(const long s, const long us)
  : m_s(s), m_us(us)
{
  Adjust();
}

Time::~Time()
{
}

void Time::Adjust()
{
  if (MEGA <= labs(m_us)) {
    m_s += m_us / MEGA;
    m_us = m_us - (m_us / MEGA) * MEGA;
  }
  if (0 < m_s && m_us < 0) {
    m_s--;
    m_us += MEGA;
  }
  if (m_s < 0 && 0 < m_us) {
    m_s++;
    m_us -= MEGA;
  }
}

void Time::Clear()
{
  m_s = m_us = 0;
}

void Time::Set()
{
  timeval tv;
  gettimeofday(&tv, 0);
  m_s = tv.tv_sec;
  m_us = tv.tv_usec;
}

void Time::Set(const double t)
{
  double s, us;
  us = modf(t, &s);
  m_s = (long)s;
  m_us = (long)(us * 1000000);
  Adjust();
}

void Time::Set(const long s, const long us)
{
  m_s = s;
  m_us = us;
  Adjust();
}

double Time::Get() const
{
  return (double)m_s + ((double)m_us) / 1000000.;
}

long Time::GetSecond() const
{
  return m_s;
}

long Time::GetMicroSecond() const
{
  return m_us;
}

std::string Time::ToString() const
{
  std::stringstream ss;
  ss << m_s << "." << m_us;
  return ss.str();
}

Time& Time::operator = (const Time& t)
{
  m_s =  t.m_s;
  m_us =  t.m_us;
  return *this;
}

bool Time::operator == (const Time& t) const
{
  return (m_s == t.m_s) && (m_us == t.m_us);
}

Time Time::operator + (const Time& t) const
{
  Time tt = *this;
  tt.m_s += t.m_s;
  tt.m_us += t.m_us;
  return tt;
}

Time Time::operator - (const Time& t) const
{
  Time tt = *this;
  tt.m_s -= t.m_s;
  tt.m_us -= t.m_us;
  return tt;
}

