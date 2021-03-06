#include "RawDataSocketInputModule.hh"
#include "RawDataFileOutputModule.hh"
#include "RawFADCUnpackerModule.hh"
#include "SharedEventBufferOutputModule.hh"
#include "Processor.hh"

using namespace JSNS2;

int receiver()
{
  RawDataSocketInputModule* input = new RawDataSocketInputModule();
  input->SetHostname("localhost");
  input->SetPort(9090);
  input->OpenStatus("/receiver.shm");

  RawFADCUnpackerModule* unpack = new RawFADCUnpackerModule();

  SharedEventBufferOutputModule* shm = new SharedEventBufferOutputModule();
  shm->SetPath("/DQMEvent.shm");
  shm->SetNword(10000000);

  RawDataFileOutputModule* output = new RawDataFileOutputModule();
  output->SetPath("/home/tkonno/disk/");
  output->SetAutoMode(true);

  Processor process;
  process.Add(input);
  process.Add(unpack);
  process.Add(shm);
  process.Add(output);
  process.Run();
  return 0;
}

int main() 
{
  return receiver();
}
