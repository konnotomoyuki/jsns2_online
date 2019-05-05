void findObject(THttpServer* http, TDirectory* cdir)
{
  TList* keys = cdir->GetListOfKeys();
  TIter next(keys);
  TKey* key = NULL;
  while((key = (TKey*)next())) {
    TObject* obj = cdir->FindObjectAny(key->GetName());
    //std::cout << obj->GetName() << "/" << cdir->GetName() << std::endl;
    if (obj->IsA()->InheritsFrom("JSNS2::Analysis::EventMetaData")) {
      JSNS2::Analysis::EventMetaData* meta = (JSNS2::Analysis::EventMetaData*)obj;
      std::cout << meta->GetRunNumber() << "." << meta->GetEventNumber() << std::endl;
      http->Register("/EventMetaData", meta);
    } else if (obj->IsA()->InheritsFrom("TH1")) {
      TH1* h = (TH1*) obj;
    } else if (obj->IsA()->InheritsFrom("TCanvas")) {
      TCanvas* c = (TCanvas*) obj;
      c->Draw();
      http->Register(Form("/%s", cdir->GetName()), c);
    } else if (obj->IsA()->InheritsFrom(TDirectory::Class())) {
      TDirectory* dir = (TDirectory*) obj;
      //std::cout << cdir->GetName() << "/" << dir->GetName() << std::endl;
      findObject(http, dir);
    }
  }
}

using namespace JSNS2;

void histserver() {
  const int BUF_SIZE = 20000000;
  THttpServer *http = new THttpServer("http:8080");
  http->ProcessRequests();

  SharedMemory shm("DQMCanvas.shm", BUF_SIZE+2000);
  MMutex mutex;
  MCond cond;
  shm.open();
  char* p = (char*)shm.map();
  mutex.set(p);
  p+= +mutex.size();
  cond.set(p);
  p+= +cond.size();

  char* buf = new char [BUF_SIZE];
  TMemFile* file = NULL;
  for (int i = 0; ; i++) {
    if (file) {
      file->Close();
      delete file;
    }
    gSystem->ProcessEvents();
    mutex.lock();
    cond.wait(mutex);
    gSystem->ProcessEvents();
    memcpy(buf, p, BUF_SIZE);
    file = new TMemFile("DQMCanvas.map", buf, BUF_SIZE);
    findObject(http, gDirectory);
    gSystem->ProcessEvents();
    mutex.unlock();
  }

}
