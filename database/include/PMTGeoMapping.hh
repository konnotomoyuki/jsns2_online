#ifndef __JSNS2_PMTGepMapping_hh
#define __JSNS2_PMTGepMapping_hh

#include <TObject.h>
#include <TNamed.h>

#include <map>

namespace JSNS2 {

  namespace DB {

    class PMTGeo : public TObject {
      
    public:
      PMTGeo() {}
      PMTGeo(int id, int location, double r, double phi, double z)
      {
	m_id = id;
	m_location = location;
	m_r = r;
	m_phi = phi;
	m_z = z;
      }
      virtual ~PMTGeo() {}
      
    public:
      Int_t GetId() const { return m_id; }
      Int_t GetLocation() const { return m_location; }
      Double_t GetR() const { return m_r; }
      Double_t GetPhi() const { return m_phi; }
      Double_t GetZ() const { return m_z; }
      void SetId(Int_t id) { m_id = id; }
      void SetLocation(Int_t location) { m_location = location; }
      void SetR(Double_t r) { m_r = r; }
      void SetPhi(Double_t phi) { m_phi = phi; }
      void SetZ(Double_t z) { m_z = z; }
      
    private:
      Int_t m_id;
      Int_t m_location;
      Double_t m_r;
      Double_t m_phi;
      Double_t m_z;
      
    public:
      ClassDef(PMTGeo, 1);
      
    };
    
    class PMTGeoMapping : public TNamed {

    public:
      PMTGeoMapping() {}
      PMTGeoMapping(const char* name);
      PMTGeoMapping(const char* name, const char* title);
      virtual ~PMTGeoMapping() {}
      
    public:
      void Add(const PMTGeo& PMT);
      std::map<int, PMTGeo>& GetPMTs() { return m_PMTs; }
      const std::map<int, PMTGeo>& GetPMTs() const { return m_PMTs; }
      const PMTGeo& Get(int id) const { return m_PMTs.at(id); }

    private:
      std::map<int, PMTGeo> m_PMTs;
      
    public:
      ClassDef(PMTGeoMapping, 1);
      
    };

  }

}

#endif 
