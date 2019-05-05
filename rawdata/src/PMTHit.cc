#include "PMTHit.hh"

ClassImp(JSNS2::PMTHit);

using namespace JSNS2;

PMTHit::PMTHit(Int_t id)
  : m_id(id), m_charge(0), m_time(0), 
    m_hight(0), m_width(0), m_pedestal(0)
{

}

PMTHit::~PMTHit()
{

}

PMTHit& PMTHit::operator=(const PMTHit& p)
{
  m_id = p.m_id;
  m_charge = p.m_charge;
  m_time = p.m_time;
  m_hight = p.m_hight;
  m_width = p.m_width;
  m_pedestal = p.m_pedestal;
  return *this;
}
