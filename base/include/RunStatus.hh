#ifndef __JSNS2_RunStatus_hh
#define __JSNS2_RunStatus_hh

namespace JSNS2 {

  struct RunStatus {
    unsigned long long count;
    unsigned int runNum;
    unsigned int eventNum;
    char type [16];        //16 bytes
    char state [16];       //16 bytes
    unsigned long long nevents;
    unsigned long long nbytes;
    unsigned int ttrig;
    unsigned int tlast;
  };

}

#endif 

