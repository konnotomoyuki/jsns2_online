#include "DataStore.hh"

using namespace JSNS2;

DataStore DataStore::g_ds;

DataStore::~DataStore() {
  Reset();
}

TObject* DataStore::Add(TObject* obj, bool allocated)
{ 
  if (obj){
    if (Find(obj->ClassName()) == NULL) {
      m_objs.insert(std::pair<std::string, TObject*>(obj->ClassName(), obj));
      return obj;
    } else {
      if (obj) delete obj;
      m_objs[obj->ClassName()] = obj;
      return obj;
    }
  }
  return NULL;
}

TObject* DataStore::Find(const char* name) {
  return (m_objs.find(name) != m_objs.end()?m_objs[name]:NULL);
}

void DataStore::Delete(const char* name) {
  if (m_objs.find(name) != m_objs.end() && m_objs[name]) {
    delete m_objs[name];
    m_objs[name] = NULL;
  }
}

void DataStore::Reset()
{ 
  for (auto& it : DataStore::Instance().GetList()) {
    if (it.second) delete it.second;
  }  
}

