#include "RawPulse.hh"

ClassImp(JSNS2::RawPulse);

using namespace JSNS2;

RawPulse::RawPulse(Int_t id, Bool_t gain)
  : m_id(id), m_gain(gain), m_charge(0), m_time(0), 
    m_hight(0), m_width(0), m_pedestal(0)
{

}

RawPulse::~RawPulse()
{

}

RawPulse& RawPulse::operator=(const RawPulse& p)
{
  m_id = p.m_id;
  m_gain = p.m_gain;
  m_charge = p.m_charge;
  m_time = p.m_time;
  m_hight = p.m_hight;
  m_width = p.m_width;
  m_pedestal = p.m_pedestal;
  return *this;
}
