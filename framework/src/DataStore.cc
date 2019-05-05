#include "DataStore.hh"

using namespace JSNS2;

DataStore DataStore::g_ds;

TObject* DataStore::Add(TObject* obj, bool allocated)
{ 
  if (obj && Find(obj->ClassName()) == NULL) {
    m_objs.insert(std::pair<std::string, TObject*>(obj->ClassName(), obj));
    return obj;
  }
  return NULL;
}

TObject* DataStore::Find(const char* name) {
  return (m_objs.find(name) != m_objs.end()?m_objs[name]:NULL);
}
