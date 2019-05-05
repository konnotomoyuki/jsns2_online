#include "PMTGeoMapping.hh"

#include <fstream>

ClassImp(JSNS2::DB::PMTGeo);
ClassImp(JSNS2::DB::PMTGeoMapping);

using namespace JSNS2;
using namespace JSNS2::DB;

PMTGeoMapping::PMTGeoMapping(const char* name)
  : TNamed(name, "PMT geometry mapping to detector")
{

}

PMTGeoMapping::PMTGeoMapping(const char* name, const char* title)
  : TNamed(name, title)
{

}

void PMTGeoMapping::Add(const PMTGeo& PMT)
{
  m_PMTs.insert(std::pair<int, PMTGeo>(PMT.GetId(), PMT));
}
