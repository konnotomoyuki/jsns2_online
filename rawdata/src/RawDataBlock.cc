#include "RawDataBlock.hh"

using namespace JSNS2;

RawDataBlock::RawDataBlock(int n, UInt_t* buf)
  : m_data(n)
{
  memcpy(Ptr(), buf, n * sizeof(int));
}
