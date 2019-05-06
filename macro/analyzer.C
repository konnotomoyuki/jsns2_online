#include "SharedEventBufferInputModule.hh"
#include "RecoPulseSimpleModule.hh"
#include "RecoPulseSimpleHistModule.hh"
#include "HitMapHistModule.hh"
#include "ViewPulseHistModule.hh"
#include "DQMHistOutputModule.hh"
#include "Processor.hh"

int analyzer()
{
  using namespace JSNS2;
  SharedEventBufferInputModule* input = new SharedEventBufferInputModule();
  input->SetPath("DQMEvent.shm");
  input->SetNword(10000000);
  RecoPulseSimpleModule* reco = new RecoPulseSimpleModule();
  RecoPulseSimpleHistModule* reco_h = new RecoPulseSimpleHistModule();
  HitMapHistModule* hitmap = new HitMapHistModule();
  ViewPulseHistModule* pulse = new ViewPulseHistModule();
  DQMHistOutputModule* output = new DQMHistOutputModule();
  output->SetPrescale(1);

  Processor process;
  process.Add(input);
  process.Add(reco);
  process.Add(reco_h);
  process.Add(hitmap);
  process.Add(pulse);
  process.Add(output);
  process.Run();
  return 0;
}


int main() 
{
  return analyzer();
}

