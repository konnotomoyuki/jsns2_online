#include <epicsExport.h>
#include <iocsh.h>

#include "devRC.h"

/** Configuration for RC IOC Shell.
 *  * Copied from
 *  * https://wiki-ext.aps.anl.gov/epics/index.php/How_to_make_your_EPICS_driver_operating_system_independent
 *  */

/** RC Configuration **/
/* parameters */
static const iocshArg rcConfigArg0 = {"libpath", iocshArgString}; /* node name */
static const iocshArg rcConfigArg1 = {"class", iocshArgString}; /* host name */
static const iocshArg rcConfigArg2 = {"name", iocshArgString}; /* port number */
static const iocshArg *rcConfigArgs[] = {&rcConfigArg0, &rcConfigArg1, &rcConfigArg2};
/* command */
static const iocshFuncDef rcConfigFuncDef = {"rcConfig", 3 /* # parameters */, rcConfigArgs};

static void rcConfigCallFunc(const iocshArgBuf *args) {
  const char * const libpath = args[0].sval;
  const char * const classname = args[1].sval;
  const char * const name = args[2].sval;
  init_rc(libpath, classname, name);
}

void RCRegisterCommands(void) {
  static int firstTime = 1;
  if (firstTime) {
    iocshRegister(&rcConfigFuncDef, rcConfigCallFunc);
    firstTime = 0;
  }
}

/** Register all Commands **/
epicsExportRegistrar(RCRegisterCommands);

