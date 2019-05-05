#ifndef _BinData_h
#define _BinData_h

#include <cstdlib>

namespace JSNS2 {

  namespace DAQ {

    struct BinTrailer {
      unsigned int magic;
    };
    
    struct BinHeader {
      unsigned short size;
      unsigned short magic;
      unsigned int run;
      unsigned int nboards;
    };
    
    const unsigned int FADC_HEADER_SIZE = 4;
    const unsigned short BIN_HEADER_MAGIC = 0x00FA;
    const unsigned int BIN_TRAILER_MAGIC = 0xCAFEBABE;
    
  }
}

#endif
