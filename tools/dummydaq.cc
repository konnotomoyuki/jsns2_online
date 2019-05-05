#ifdef __CINT__
int dummydaq()
{
#else 

#include "DummyEventModule.hh"
#include "RawDataSocketOutputModule.hh"
#include "Processor.hh"

int main() 
{
#endif
  using namespace JSNS2;

  DummyEventModule* dummy = new DummyEventModule();
  dummy->SetRunNumber(1010);
  dummy->SetFilePath("/home/usr/tkonno/jsns/EventDisplayEVE/uniformPositron10MeV-Tree.root");
  RawDataSocketOutputModule* output = new RawDataSocketOutputModule();
  output->SetHostname("localhost");
  output->SetPort(9090);

  Processor process;
  process.Add(dummy);
  process.Add(output);
  process.Run();
}
