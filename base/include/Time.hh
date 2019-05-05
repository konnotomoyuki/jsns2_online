#ifndef _JSNS2_Time_h
#define _JSNS2_Time_h

#include <string>

namespace JSNS2 {

  class Time {
    
  public:
    explicit Time();
    Time(const double);
    Time(const long, const long);
    ~Time();
      
  private:
    void Adjust();
    
  public:
    void Clear();
    void Set();
    void Set(const double);
    void Set(const long, const long);
    double Get() const;
    long GetSecond() const;
    long GetMicroSecond() const;
    std::string ToString() const;
    
  public:
    Time& operator = (const Time&);
    bool operator == (const Time&) const;
    Time operator + (const Time&) const;
    Time operator - (const Time&) const;
    
  private:
    long m_s;
    long m_us;
    
  };

}

#endif
