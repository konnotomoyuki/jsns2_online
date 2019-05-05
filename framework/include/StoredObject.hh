#ifndef _JSNS2_StoredObject_h_
#define _JSNS2_StoredObject_h_

#include "DataStore.hh"

#include <TObject.h>
#include <vector>

namespace JSNS2 {

  template <typename T>
  class StoredObject {
    
  public:
    static StoredObject Create()
    {
      return StoredObject(new T(), true);
    }
    static bool IsRegistered()
    {
      return DataStore::Instance().Find(T::Class_Name()) != NULL;
    }
    
  public:
    StoredObject() : m_ptr(NULL)
    {
      m_ptr = (T*)DataStore::Instance().Find(T::Class_Name());
    }
    StoredObject(T* obj, bool allocated = false) : m_ptr(obj)
    {
      if (m_ptr) {
	DataStore::Instance().Add(m_ptr, allocated);
      }
    }
    
    virtual ~StoredObject()
    {
	
    }

    public:      
    inline T& operator *()  const {
      return *operator->();
    }
    inline T* operator ->() const {
      return (T*)(m_ptr);
    }
    operator bool() const {
      return m_ptr != NULL;
    }
    T* ptr() { return m_ptr; }
    
  private:
    T* m_ptr;
    
  };

}

#endif
