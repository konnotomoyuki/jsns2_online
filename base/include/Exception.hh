#ifndef __JSNS2_Exception_hh
#define __JSNS2_Exception_hh

#include <string>

namespace JSNS2 {

  class Exception {
    
  public:
    Exception();
    Exception(const std::string&);
    virtual ~Exception();
    
  public:
    virtual const char* What() const;
    
  protected:
    std::string m_comment;
    
  };

}

#endif
