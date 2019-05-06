#include "Processor.hh"

#include "Module.hh"
#include "StoredObject.hh"
#include "EventMetaData.hh"

#include "LogFile.hh"
#include "Exception.hh"

using namespace JSNS2;

void Processor::Run(UInt_t maxEvent) 
{
  try {
    for (auto& mod : m_mods) {
      mod->Initialize();
    }
  } catch (JSNS2::Exception& e) {
    LogFile::fatal(e.What());
    return ;
  }
  try {
    Long64_t runNum = -1;
    while (true) {
      LogFile::debug("starting being run");
      for (auto& mod : m_mods) {
	mod->BeginRun();
      }
      StoredObject<EventMetaData> ev;
      if (!ev) {
	LogFile::fatal("No Event meta data is available");
	break;
      } 
      UInt_t count = 0;
      bool runend = false;
      while (true) {
	for (auto& mod : m_mods) {
	  if (!mod->ProcessEvent()) {
	    runend = true;
	    break;
	  }
	}
	if (runend) break;
	if (runNum != ev->GetRunNumber()) {
	  LogFile::notice("New run %d is started", ev->GetRunNumber());
	  if (runNum > 0) {
	    runNum = ev->GetRunNumber();
	    break;
	  }
	}
	runNum = ev->GetRunNumber();
	count++;
	if ((count < 10) |
	    (count > 9 && count < 100 && count % 10 == 0) |
	    (count > 99 && count < 1000 && count % 100 == 0) |
	    (count % 1000 == 0)) {
	  LogFile::info( "%u events processed..", count);
	}
	if (maxEvent > 0 && count == maxEvent) break;
      }
      for (auto& mod : m_mods) {
	mod->EndRun();
      }
      if (runend ||(maxEvent > 0 && count == maxEvent)) break;
    }
  } catch (JSNS2::Exception& e) {
    LogFile::error("Exception detected. terminating process.. %s", e.What());
  }
  try {
    for (auto& mod : m_mods) {
      mod->Finalize();
    }
  } catch (JSNS2::Exception& e) {
    LogFile::fatal(e.What());
  }
}

void Processor::Add(Module* mod)
{ 
  if (mod) {
    mod->SetProcessor(this);
    m_mods.push_back(mod);
  }
} 
