#ifndef _JSNS2_PVManager_h
#define _JSNS2_PVManager_h

#include <string>
#include <vector>
#include <map>

namespace JSNS2 {

  class PVManager {

  public:
    PVManager() {}
    virtual ~PVManager() {}

  public:
    virtual bool putRCState(const char* newstate) = 0;
    virtual bool putPV(const char* name, int val) = 0;
    virtual bool putPV(const char* name, float val) = 0;
    virtual bool putPV(const char* name, const char* val) = 0;

  };

}

#endif
