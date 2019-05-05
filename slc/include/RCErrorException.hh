#ifndef _JSNS2_RCErrorException_hh
#define _JSNS2_RCErrorException_hh

#include "Exception.hh"

namespace JSNS2 {

  class RCErrorException : public Exception {
    
  public:
    RCErrorException() {}
    RCErrorException(const std::string& comment, ...);
    virtual ~RCErrorException() {}
    
  };
  
}

#endif
