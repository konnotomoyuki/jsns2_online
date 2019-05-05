#include "CanvasModule.hh"

using namespace JSNS2;

CanvasModule::CanvasModule(const char* name)
  : Module (name)
{

}

CanvasModule::~CanvasModule()
{

}

Bool_t CanvasModule::Initialize()
{
  return true;
}

Bool_t CanvasModule::BeginRun()
{
  return true;
}

Bool_t CanvasModule::ProcessEvent()
{
  return true;
}

Bool_t CanvasModule::EndRun()
{
  return true;
}

Bool_t CanvasModule::Finalize()
{
  return true;
}

TCanvas* CanvasModule::AddCanvas(TCanvas* c)
{
  m_cs.push_back(c);
  return c;
}
