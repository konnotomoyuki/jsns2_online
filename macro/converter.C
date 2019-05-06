#include "RawDataFileInputModule.hh"
#include "RootFileOutputModule.hh"
#include "Processor.hh"

using namespace JSNS2;

int converter(const char* pathin, const char* pathout) {

  RawDataFileInputModule* input = new RawDataFileInputModule();
  input->SetPath(pathin);
  RootFileOutputModule* output = new RootFileOutputModule();
  output->SetPath(pathout);

  Processor process;
  process.Add(input);
  process.Add(output);
  process.Run();

  return 0;
}

int main(int argc, const char** argv)
{
  const char* pathin = argv[1];
  const char* pathout = argv[2];
  return converter(pathin, pathout);
}
