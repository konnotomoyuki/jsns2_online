#include "HttpServerModule.hh"

#include <TSystem.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TList.h>

using namespace JSNS2;

HttpServerModule::HttpServerModule() : Module ("HttpServer")
{
  m_hostname = "http:8080?monitoring=2000";
}

HttpServerModule::~HttpServerModule()
{

}

Bool_t HttpServerModule::Initialize()
{
  m_http = new THttpServer(m_hostname.c_str());
  m_http->ProcessRequests();
  return true;
}

Bool_t HttpServerModule::BeginRun()
{
  return true;
}

Bool_t HttpServerModule::ProcessEvent()
{
  findObject(m_http, gDirectory);
  gSystem->ProcessEvents();
  return true;
}

Bool_t HttpServerModule::EndRun()
{
  return true;
}

Bool_t HttpServerModule::Finalize()
{
  return true;
}

void HttpServerModule::findObject(THttpServer* http, TDirectory* cdir)
{
  TList* keys = cdir->GetListOfKeys();
  TIter next(keys);
  TKey* key = NULL;
  while((key = (TKey*)next())) {
    TObject* obj = cdir->FindObjectAny(key->GetName());
    if (obj->IsA()->InheritsFrom("TH1")) {
      TH1* h = (TH1*) obj;
      http->Register(Form("/%s", cdir->GetName()), h);
    } else if (obj->IsA()->InheritsFrom("TCanvas")) {
      TCanvas* c = (TCanvas*) obj;
      c->Draw();
      http->Register(Form("/%s", cdir->GetName()), c);
    } else if (obj->IsA()->InheritsFrom(TDirectory::Class())) {
      TDirectory* dir = (TDirectory*) obj;
      findObject(http, dir);
    }
  }
}
