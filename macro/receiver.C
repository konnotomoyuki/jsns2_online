#ifdef __CINT__
int receiver()
{
#else 

#include "RawDataSocketInputModule.hh"
#include "SharedEventBufferOutputModule.hh"
#include "Processor.hh"

int main() 
{
#endif
  using namespace JSNS2;
  RawDataSocketInputModule* input = new RawDataSocketInputModule();
  input->SetHostname("localhost");
  input->SetPort(9090);
  input->OpenStatus("/receiver.shm");

  SharedEventBufferOutputModule* output = new SharedEventBufferOutputModule();
  output->SetPath("/DQMEvent.shm");
  output->SetNword(10000000);

  Processor process;
  process.Add(input);
  process.Add(output);
  process.Run();
  return 0;
}
