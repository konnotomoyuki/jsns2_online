#ifndef _JSNS2_DBObject_h
#define _JSNS2_DBObject_h

#include <TFile.h>

namespace JSNS2 {

  template <typename T> 
  class DBObject {
    
  public:
    DBObject() {}
    DBObject(const char* name) {
      if (!g_stored) {
	std::string filepath = Form("%s/db/%s.root", getenv("JSNS2_HOME"), name);
	TFile* dbfile = new TFile(filepath.c_str());
	g_stored = (T*)dbfile->Get(name)->Clone();
	dbfile->Close();
	delete dbfile;
      }
    }
    DBObject(const char* name, const char* filepath) {
      if (!g_stored) {
	TFile* dbfile = new TFile(filepath);
	g_stored = (T*)dbfile->Get(name);
	dbfile->Close();
	delete dbfile;
      }
    }
    virtual ~DBObject() {
      
    }
    
  public:
    inline T& operator *()  const {
      return *operator->();
    }
    inline T* operator ->() const {
      return (T*)(g_stored);
    }
    operator bool() const {
      return g_stored != NULL;
    }
    
  private:
    static T* g_stored;
    
  };
  
  template <typename T>
  T* DBObject<T>::g_stored = NULL;

}

#endif  
