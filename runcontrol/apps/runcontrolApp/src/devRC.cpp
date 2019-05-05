#include "devRC.h"

#include "RCCallback.hh"
#include "RCErrorException.hh"
#include "PVHandler.hh"

#include "Mutex.hh"
#include "PThread.hh"
#include "LogFile.hh"
#include "StringUtil.hh"

#include <recGbl.h>
#include <dbAccess.h>
#include <dbScan.h>
#include <dbEvent.h>
#include <dbDefs.h>
#include <devSup.h>

#include <iostream>
#include <string>
#include <map>
#include <cstring>
#include <cstdlib>
#include <dlfcn.h>
#include <unistd.h>

using namespace JSNS2;

PVManager* g_man = NULL;
Mutex g_mutex;
RCCallback* g_callback = NULL;
stringinRecord *g_record_rcstate = NULL;
std::map<std::string, longinRecord*> g_iinrecords;
std::map<std::string, aiRecord*> g_finrecords;
std::map<std::string, stringinRecord*> g_sinrecords;

extern "C" {

  class Updater {
  public:
    Updater(RCCallback* callback) 
      : m_callback(callback) {}

  public:
    void run() 
    {
      while (true) {
	sleep(m_callback->getTimeout());
	try {
	  m_callback->timeout();
	} catch (const RCErrorException& e) {
	  LogFile::error(e.What());
	  g_man->putRCState("ERROR");
	}
      }
    }
  private:
    RCCallback* m_callback;

  };

  class IOCPVManager : public JSNS2::PVManager {

  public:
    IOCPVManager() {}
    virtual ~IOCPVManager() {}

  public:
    virtual bool putRCState(const char* newstate)
    {
      std::string state = g_callback->getState();
      g_callback->setState(newstate);
      LogFile::info("state transition (%s>>%s)", state.c_str(), newstate);
      if (g_record_rcstate) {
	sprintf(g_record_rcstate->val, g_callback->getState().c_str());
	g_record_rcstate->udf = TRUE;//FALSE;
	scanIoRequest(*(IOSCANPVT*)(g_record_rcstate->dpvt));
	return true;
      }
      return false;
    }
    virtual bool putPV(const char* name, int val)
    {
      std::string vname = g_callback->getName() + StringUtil::form(":%s", name);
      if (g_iinrecords.find(vname) != g_iinrecords.end()) {
	g_iinrecords[vname]->val = val;
	g_iinrecords[vname]->udf = TRUE;//FALSE;
	scanIoRequest(*(IOSCANPVT*)(g_iinrecords[vname]->dpvt));
	return true;
      }
      return false;
    }
    virtual bool putPV(const char* name, float val)
    {
      std::string vname = g_callback->getName() + StringUtil::form(":%s", name);
      if (g_finrecords.find(vname) != g_finrecords.end()) {
	g_finrecords[vname]->val = val;
	g_finrecords[vname]->udf = TRUE;
	scanIoRequest(*(IOSCANPVT*)(g_finrecords[vname]->dpvt));
	return true;
      }
      return false;
    }
    virtual bool putPV(const char* name, const char* val)
    {
      std::string vname = g_callback->getName() + StringUtil::form(":%s", name);
      if (g_sinrecords.find(vname) != g_sinrecords.end()) {
	g_sinrecords[vname]->val[0] = 0;
	strcpy(g_sinrecords[vname]->val, val);
	g_sinrecords[vname]->udf = TRUE;//FALSE;
	scanIoRequest(*(IOSCANPVT*)(g_sinrecords[vname]->dpvt));
	return true;
      }
      return false;
    }
  };

  typedef void* (*getclass_t)(void);

  bool checkinRecord(unsigned int type, unsigned int scan, void* record)
  {
    if (type != INST_IO) {
      recGblRecordError(S_db_badField, record, "init_record Illegal INP field. INT_IO only."); 
      return false;
    }
    if (scan != SCAN_IO_EVENT) {
      recGblRecordError(S_db_badField, record, "init_record Illegal SCAN field. IO/INTR scans only.");
      return false;
    }
    return true;
  }
  
  bool checkoutRecord(unsigned int scan, void* record)
  {
    if (scan != SCAN_PASSIVE) {
      recGblRecordError(S_db_badField, (void*)record,
			"init_record Illegal SCAN field. PASSIVE scans only.");
      return false;
    }
    return true;
  }
  
  long init_rc(const char* libname, const char* classname, const char* name)
  {
    void* handle = dlopen(libname, RTLD_LAZY);
    if (!handle) {
      LogFile::fatal("dlopen: %s", dlerror());
      exit(1);
    }
    char* error = NULL;
    void* (*func)(void);
    std::string getclass = StringUtil::form("Get%sCallback", classname);
    LogFile::debug("loading class from %s:%s", libname, getclass.c_str());
    func = (void* (*)(void))dlsym(handle, getclass.c_str());
    if ((error = dlerror()) != NULL) {
      LogFile::fatal("dlsym: %s", error);
      exit(1);
    }
    g_callback = (RCCallback*) func();
    g_man = new IOCPVManager();
    g_callback->setPVManager(g_man);
    LogFile::debug("set callback name = %s", name);
    g_callback->setName(name);
    g_callback->init();
    PThread(new Updater(g_callback));
    return 0;
  }
  
  long get_ioint_info_rcstate_stringin(int cmd, stringinRecord* record, IOSCANPVT* pvt)
  {
    if (cmd != 0) return 0;
    if (!record->dpvt) return 1;
    printf("get ioint info rcstate %s\n", record->name);
    *pvt = *((IOSCANPVT*)record->dpvt);
    return 0;
  }
  
  long init_rcstate_stringin(stringinRecord* record)
  {
    if(checkinRecord(record->inp.type, record->scan, record)) {
      IOSCANPVT* pvt = new IOSCANPVT;
      scanIoInit(pvt);
      record->dpvt = pvt;
      LogFile::info("init rcstate=%s", record->name);
      g_record_rcstate = record;
      return 0;
    }
    return S_db_badField;
  }

  long read_rcstate_stringin(stringinRecord* record)
  {
    std::string val = g_callback->getState();
    memset(record->val, 0, strlen(record->val));
    strcpy(record->val, val.c_str());
    record->udf = FALSE;
    return 0;
  }

  long init_rcrequest_stringout(stringoutRecord *record)
  {
    record->udf = TRUE;
    if(checkoutRecord(record->scan, record)){
      LogFile::info("init rcrequest=%s", record->name);
      return 0;
    }
    return S_db_badField;
  }
  
  long write_rcrequest_stringout(stringoutRecord *record)
  {
    g_callback->perform(record->val);
    return 0;
  }

  long init_pvhandler_longin(longinRecord *record)
  {
    if(checkinRecord(record->inp.type, record->scan, record)) {
      IOSCANPVT* pvt = new IOSCANPVT;
      scanIoInit(pvt);
      record->dpvt = pvt;
      LogFile::info("init pvhandler=%s", record->name);
      g_iinrecords.insert(std::pair<std::string, longinRecord*>(record->name, record));
      return 0;
    }
    return S_db_badField;
  }

  long get_ioint_info_pvhandler_longin(int cmd, longinRecord* record, IOSCANPVT* pvt)
  {
    if (cmd != 0) return 0;
    if (!record->dpvt) return 1;
    printf("get ioint info pvhandler %s\n", record->name);
    *pvt = *((IOSCANPVT*)record->dpvt);
    return 0;
  }

  long read_pvhandler_longin(longinRecord *record)
  {
    g_mutex.Lock();
    PVHandler* handler = g_callback->getHandler(record->name);
    if (handler && handler->getType() == PVHandler::INT) {
      PVHandlerInt* h = (PVHandlerInt*) handler;
      record->val = h->get();
      g_mutex.Unlock();
      record->udf = FALSE;
    } else {
      g_mutex.Unlock();
    }
    return 0;
  }
  
  long init_pvhandler_ai(aiRecord *record)
  {
    if(checkinRecord(record->inp.type, record->scan, record)) {
      IOSCANPVT* pvt = new IOSCANPVT;
      scanIoInit(pvt);
      record->dpvt = pvt;
      LogFile::info("init pvhandler=%s", record->name);
      g_finrecords.insert(std::pair<std::string, aiRecord*>(record->name, record));
      return 0;
    }
    return S_db_badField;
  }
  
  long get_ioint_info_pvhandler_ai(int cmd, aiRecord* record, IOSCANPVT* pvt)
  {
    if (cmd != 0) return 0;
    if (!record->dpvt) return 1;
    printf("get ioint info pvhandler %s\n", record->name);
    *pvt = *((IOSCANPVT*)record->dpvt);
    return 0;
  }
  
  long read_pvhandler_ai(aiRecord *record)
  {
    g_mutex.Lock();
    PVHandler* handler = g_callback->getHandler(record->name);
    if (handler && handler->getType() == PVHandler::FLOAT) {
      PVHandlerFloat* h = (PVHandlerFloat*) handler;
      record->val = h->get();
      record->rval = h->get();
      g_mutex.Unlock();
      record->udf = FALSE;
    } else {
      g_mutex.Unlock();
    }
    return 0;
  }
  
  long init_pvhandler_stringin(stringinRecord *record)
  {
    if(checkinRecord(record->inp.type, record->scan, record)) {
      IOSCANPVT* pvt = new IOSCANPVT;
      scanIoInit(pvt);
      record->dpvt = pvt;
      LogFile::info("init pvhandler=%s", record->name);
      g_sinrecords.insert(std::pair<std::string, stringinRecord*>(record->name, record));
      return 0;
    }
    return S_db_badField;
  }
  
  long get_ioint_info_pvhandler_stringin(int cmd, stringinRecord* record, IOSCANPVT* pvt)
  {
    if (cmd != 0) return 0;
    if (!record->dpvt) return 1;
    printf("get ioint info pvhandler %s\n", record->name);
    *pvt = *((IOSCANPVT*)record->dpvt);
    return 0;
  }
  
  long read_pvhandler_stringin(stringinRecord *record)
  {
    g_mutex.Lock();
    PVHandler* handler = g_callback->getHandler(record->name);
    if (handler && handler->getType() == PVHandler::STRING) {
      PVHandlerString* h = (PVHandlerString*) handler;
      record->val[0] = 0;
      strcpy(record->val, h->get().c_str());
      g_mutex.Unlock();
      record->udf = FALSE;
    } else {
      g_mutex.Unlock();
    }
    return 0;
  }
  
  long init_pvhandler_stringout(stringoutRecord *record)
  {
    record->udf = TRUE;
    if(checkoutRecord(record->scan, record)){
      PVHandler* handler = g_callback->getHandler(record->name);
      if (handler && handler->getType() == PVHandler::STRING) {
	PVHandlerString* h = (PVHandlerString*) handler;
	memset(record->val, 0, strlen(record->val));
	strcpy(record->val, h->get().c_str());
	LogFile::info("init write PV %s<<%s", record->name, record->val);
	record->udf = FALSE;
      }
      return 0;
    }
    return S_db_badField;
  }
  
  long write_pvhandler_stringout(stringoutRecord *record)
  {
    g_mutex.Lock();
    PVHandler* handler = g_callback->getHandler(record->name);
    if (handler && handler->getType() == PVHandler::STRING) {
      PVHandlerString* h = (PVHandlerString*) handler;
      if (h->handleSet(record->val)) {
	std::cout << "write pv " << record->name << "<<" << h->get() << std::endl;
      }
      g_mutex.Unlock();
      record->udf = FALSE;
    } else {
      g_mutex.Unlock();
    }
    return 0;
  }

  long init_pvhandler_longout(longoutRecord *record)
  {
    record->udf = TRUE;
    if(checkoutRecord(record->scan, record)){
      PVHandler* handler = g_callback->getHandler(record->name);
      if (handler && handler->getType() == PVHandler::INT) {
	PVHandlerInt* h = (PVHandlerInt*) handler;
	record->val = h->get();
	LogFile::info("init write PV %s<<%s", record->name, record->val);
	record->udf = FALSE;
      }
      return 0;
    }
    return S_db_badField;
  }
  
  long write_pvhandler_longout(longoutRecord *record)
  {
    g_mutex.Lock();
    PVHandler* handler = g_callback->getHandler(record->name);
    if (handler && handler->getType() == PVHandler::INT) {
      PVHandlerInt* h = (PVHandlerInt*) handler;
      if (h->handleSet(record->val)) {
	std::cout << "write pv " << record->name << "<<" << h->get() << std::endl;
      }
      g_mutex.Unlock();
      record->udf = FALSE;
    } else {
      g_mutex.Unlock();
    }
    return 0;
  }

  long init_pvhandler_ao(aoRecord* record)
  {
    record->udf = TRUE;
    if(checkoutRecord(record->scan, record)){
      PVHandler* handler = g_callback->getHandler(record->name);
      if (handler && handler->getType() == PVHandler::FLOAT) {
	PVHandlerFloat* h = (PVHandlerFloat*) handler;
	record->val = h->get();
	LogFile::info("init write PV %s<<%s", record->name, record->val);
	record->udf = FALSE;
      }
      return 0;
    }
    return S_db_badField;
  }
  
  long write_pvhandler_ao(aoRecord* record)
  {
    g_mutex.Lock();
    PVHandler* handler = g_callback->getHandler(record->name);
    if (handler && handler->getType() == PVHandler::FLOAT) {
      PVHandlerFloat* h = (PVHandlerFloat*) handler;
      if (h->handleSet(record->val)) {
	std::cout << "write pv " << record->name << "<<" << h->get() << std::endl;
      }
      g_mutex.Unlock();
      record->udf = FALSE;
    } else {
      g_mutex.Unlock();
    }
    return 0;
  }

}
