#ifdef __CINT__
int analyzer()
{
#else 

#include "SharedEventBufferInputModule.hh"
#include "CalcPulseModule.hh"
#include "DQMHistOutputModule.hh"
#include "Processor.hh"

int main() 
{
#endif

  using namespace JSNS2;
  SharedEventBufferInputModule* input = new SharedEventBufferInputModule();
  input->SetPath("DQMEvent.shm");
  input->SetNword(10000000);
  CalcPulseModule* calc = new CalcPulseModule();
  DQMHistOutputModule* output = new DQMHistOutputModule();
  output->SetPrescale(1);
  Processor process;
  process.Add(input);
  process.Add(calc);
  process.Add(output);
  process.Run();
}

