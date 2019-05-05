using namespace JSNS2;

const int BUF_SIZE = 20000000;

class MyTimer : public TTimer
{

private:

private:
  IDDisplay3D* m_display;
  JSNS2::SharedMemory shm;
  MMutex mutex;
  MCond cond;
  char* p;
  char* buf;

public:
  MyTimer(IDDisplay3D* display)
    : TTimer(1000), m_display(display),
      shm("DQMHist.shm", BUF_SIZE+2000)
  {
    buf = new char[BUF_SIZE];
    shm.Open();
    p = (char*)shm.Map();
    mutex.Set(p);
    p+= +mutex.GetSize();
    cond.Set(p);
    p+= +cond.GetSize();
  }
  
  virtual Bool_t Notify()
  {
    // stop timer
    TurnOff();
    mutex.Lock();
    cond.Wait(mutex);
    memcpy(buf, p, BUF_SIZE);
    mutex.Unlock();
    TMemFile* file = new TMemFile("DQMHist.map", buf, BUF_SIZE);
    TDirectory* dir = gDirectory;
    gStyle->SetPalette(55);
    TH1* h = (TH1*)dir->FindObjectAny("h_PMT_charge_low");
    if (h) {
      LogFile::debug("Updating display");
      m_display->Update(h);
      gEve->GetDefaultGLViewer()->RequestDraw();
      gEve->DoRedraw3D();
      gEve->GetGlobalScene()->Changed();
    }
    file->Close();
    delete file;
    // restart timer
    SetTime(5000);
    Reset();
    TurnOn();
    return true;
  }
  
};

void online_display() 
{
  //gStyle->SetPalette(1);

  TEveManager* eve = TEveManager::Create();
  TGeoManager *geo = new TGeoManager("ellipsoid", "");
  geo->SetNsegments(10);

  IDDisplay3D* display = new IDDisplay3D("h_PMTmap_charge", "PMT charge map", "Charge [ADC sum]");
  display->SetEveManager(eve);
  display->SetGeoManager(geo);

  const int nPMTs = display->GetNPMTs();
  for (int i = 0; i < nPMTs; i++) {
    display->SetBinContent(i+1, i);
  }
  display->Draw();

  TGeoNode* node = geo->GetTopNode();
  TEveGeoTopNode* en = new TEveGeoTopNode(geo, node);
  gEve->AddGlobalElement(en);
  gEve->Redraw3D(kTRUE);

  std::cout << "set timer" << std::endl;
  MyTimer* timer = new MyTimer(display);
  timer->SetTime(5000);
  timer->Reset();
  timer->TurnOn();
}
