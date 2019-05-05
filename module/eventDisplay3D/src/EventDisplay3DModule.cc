#include "EventDisplay3DModule.hh"

#include <TH1.h>
#include <TDirectory.h>

using namespace JSNS2;

EventDisplay3DModule::EventDisplay3DModule(TEveManager* eve, TGeoManager* geom)
  : Module ("EventDisplay3D")
{
  m_display = new IDDisplay3D("3d_charge_low", "Display for charge", "Charge [ADC]");
  m_display->SetEveManager(eve);
  m_display->SetGeoManager(geom);
}

EventDisplay3DModule::~EventDisplay3DModule()
{

}

Bool_t EventDisplay3DModule::Initialize()
{
  return true;
}

Bool_t EventDisplay3DModule::BeginRun()
{
  //StoredObject<Data::Event> ev;
  return true;
}

Bool_t EventDisplay3DModule::ProcessEvent()
{
  // StoredObject<Data::Event> ev;
  TDirectory* dir = gDirectory;
  TH1* h = (TH1*)dir->FindObjectAny("h_PMT_charge_low");
  if (h) {
    m_display->Update(h);
  }
  return true;
}

Bool_t EventDisplay3DModule::EndRun()
{
  return true;
}

Bool_t EventDisplay3DModule::Finalize()
{
  return true;
}


