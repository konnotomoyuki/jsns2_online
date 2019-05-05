#ifndef _EPICS_RC_MSG_C_WRAPPER_H_
#define _EPICS_RC_MSG_C_WRAPPER_H_

#include <aiRecord.h>
#include <aoRecord.h>
#include <longinRecord.h>
#include <longoutRecord.h>
#include <stringinRecord.h>
#include <stringoutRecord.h>
#include <dbScan.h>

#ifdef __cplusplus
extern "C" {
#endif

  long init_rc(const char* libname, const char* classname, const char* name);
  long init_rcstate_stringin(stringinRecord* record);
  long get_ioint_info_rcstate_stringin(int cmd, stringinRecord* record, IOSCANPVT* pvt);
  long read_rcstate_stringin(stringinRecord* record);
  long init_rcrequest_stringout(stringoutRecord *record);
  long write_rcrequest_stringout(stringoutRecord *record);
  long init_pvhandler_longin(longinRecord* record);
  long get_ioint_info_pvhandler_longin(int cmd, longinRecord* record, IOSCANPVT* pvt);
  long get_ioint_info_pvhandler_stringin(int cmd, stringinRecord* record, IOSCANPVT* pvt);
  long get_ioint_info_pvhandler_ai(int cmd, aiRecord* record, IOSCANPVT* pvt);
  long read_pvhandler_longin(longinRecord* record);
  long init_pvhandler_ai(aiRecord* record);
  long read_pvhandler_ai(aiRecord* record);
  long init_pvhandler_stringin(stringinRecord* record);
  long read_pvhandler_stringin(stringinRecord* record);
  long init_pvhandler_stringout(stringoutRecord *record);
  long write_pvhandler_stringout(stringoutRecord *record);
  long init_pvhandler_longout(longoutRecord *record);
  long write_pvhandler_longout(longoutRecord *record);
  long init_pvhandler_ao(aoRecord *record);
  long write_pvhandler_ao(aoRecord *record);
  
#ifdef __cplusplus
}
#endif

#endif
