#ifndef _JSNS2_DataStore_h
#define _JSNS2_DataStore_h

#include <TObject.h>

#include <map>
#include <string>

namespace JSNS2 {
  
  class DataStore {
    
  public:
    static DataStore& Instance() { return g_ds; }
    
  private:
    static DataStore g_ds;
    
  public:
    virtual ~DataStore();
    
  private:
    DataStore() {}
    
  public:
    TObject* Add(TObject* obj, bool allocated = false);
    TObject* Find(const char* name);
    std::map<std::string, TObject*>& GetList() { return m_objs; }
    void Delete(const char* name);
    void Reset();

  private:
    std::map<std::string, TObject*> m_objs;
    
  };

}

#endif

