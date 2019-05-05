#include "devRC.h"

#include <epicsExport.h>
#include <dbAccess.h>
#include <dbScan.h>
#include <devSup.h>
#include <recSup.h>
#include <recGbl.h>

struct
{
  long number;
  DEVSUPFUN dev_report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN read_stringin;
} dev_rcstate_stringin = {
  5, NULL, NULL, init_rcstate_stringin,
  get_ioint_info_rcstate_stringin, 
  read_rcstate_stringin
};

struct
{
  long number;
  DEVSUPFUN dev_report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN write_stringout;
} dev_rcrequest_stringout = {
  5, NULL, NULL, init_rcrequest_stringout,
  NULL, write_rcrequest_stringout
};

struct
{
  long number;
  DEVSUPFUN dev_report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN read_longin;
} dev_pvhandler_longin = {
  5, NULL, NULL, init_pvhandler_longin,
  get_ioint_info_pvhandler_longin, 
  read_pvhandler_longin
};

struct
{
  long number;
  DEVSUPFUN dev_report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN read_ai;
  DEVSUPFUN special_linconv;
} dev_pvhandler_ai = {
  6, NULL, NULL, init_pvhandler_ai,
  get_ioint_info_pvhandler_ai, 
  read_pvhandler_ai,
  NULL
};

struct
{
  long number;
  DEVSUPFUN dev_report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN read_longin;
} dev_pvhandler_stringin = {
  5, NULL, NULL, init_pvhandler_stringin,
  get_ioint_info_pvhandler_stringin,
  read_pvhandler_stringin
};

struct
{
  long number;
  DEVSUPFUN dev_report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN write_stringout;
} dev_pvhandler_stringout = {
  5, NULL, NULL, init_pvhandler_stringout,
  NULL, write_pvhandler_stringout
};

struct
{
  long number;
  DEVSUPFUN dev_report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN write_longout;
} dev_pvhandler_longout = {
  5, NULL, NULL, init_pvhandler_longout,
  NULL, write_pvhandler_longout
};

struct
{
  long number;
  DEVSUPFUN dev_report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN write_ao;
} dev_pvhandler_ao = {
  5, NULL, NULL, init_pvhandler_ao,
  NULL, write_pvhandler_ao
};

epicsExportAddress(dset, dev_rcstate_stringin);
epicsExportAddress(dset, dev_rcrequest_stringout);
epicsExportAddress(dset, dev_pvhandler_stringin);
epicsExportAddress(dset, dev_pvhandler_longin);
epicsExportAddress(dset, dev_pvhandler_ai);
epicsExportAddress(dset, dev_pvhandler_stringout);
epicsExportAddress(dset, dev_pvhandler_longout);
epicsExportAddress(dset, dev_pvhandler_ao);
