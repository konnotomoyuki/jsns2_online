#include "RawDataBlock.hh"

using namespace JSNS2;

RawDataBlock::RawDataBlock(int n, UInt_t* buf)
  : m_data(n)
{
  memcpy(Ptr(), buf, n * sizeof(int));
}

RawDataBlock::RawDataBlock(const RawDataBlock& block)
  : m_data(block.m_data.size())
{
  memcpy(Ptr(), block.Ptr(), block.m_data.size() * sizeof(int));
}
