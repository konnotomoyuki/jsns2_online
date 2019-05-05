#include "PMTHitSummary.hh"

ClassImp(JSNS2::PMTHitSummary);

using namespace JSNS2;

PMTHitSummary::PMTHitSummary()
{

}

PMTHitSummary::~PMTHitSummary()
{

}

void PMTHitSummary::Add(Int_t id, const PMTHit& hit, Bool_t gain)
{
  GetList(gain).insert(std::pair<int, PMTHit>(id, hit));
}

PMTHit& PMTHitSummary::Get(Int_t id, Bool_t gain)
{
  return GetList(gain)[id];
}

std::map<int, PMTHit>& PMTHitSummary::GetList(Bool_t gain)
{
  return (gain?m_hits_high:m_hits_low);
}
      
