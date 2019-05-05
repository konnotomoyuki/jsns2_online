#include "HistModule.hh"

using namespace JSNS2;

HistModule::HistModule(const char* name)
  : Module (name)
{

}

HistModule::~HistModule()
{

}

Bool_t HistModule::Initialize()
{
  return true;
}

Bool_t HistModule::BeginRun()
{
  return true;
}

Bool_t HistModule::ProcessEvent()
{
  return true;
}

Bool_t HistModule::EndRun()
{
  return true;
}

Bool_t HistModule::Finalize()
{
  return true;
}

TH1* HistModule::AddHist(TH1* h)
{
  m_hists.push_back(h);
  return h;
}
