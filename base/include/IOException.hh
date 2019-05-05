#ifndef _JSNS2_IOException_hh
#define _JSNS2_IOException_hh

#include "Exception.hh"

namespace JSNS2 {

  class IOException : public Exception {
    
  public:
    IOException() {}
    IOException(const std::string& comment, ...);
    virtual ~IOException() {}
    
  };
  
}

#endif
