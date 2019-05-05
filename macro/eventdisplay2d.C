using namespace JSNS2;

void eventdisplay2d() {
  THttpServer *http = new THttpServer("http:8080");
  http->ProcessRequests();

  TCanvas* c = new TCanvas("c", "c", 1280, 640);
  IDDisplay2D* display = new IDDisplay2D("h_PMTmap_charge", "PMT charge map", "Charge [ADC sum]");
  const int nPMTs = display->GetNPMTs();
  for (int i = 0; i < nPMTs; i++) {
    display->SetBinContent(i, i);
  }

  http->Register("/c", c);
  ULong64_t count = 0;
  while (true) {
    for (int i = 0; i < nPMTs; i++) {
      display->SetBinContent(i, gRandom->Gaus(100,10));
    }
    std::cout << count << " update 1" << std::endl;
    c->cd();
    display->Draw();
    c->Modified();
    c->Update();
    for (int i = 0; i < 40; i++ ) {
      gSystem->ProcessEvents();
      usleep(1000*100); // 100ms
    }
  }

}
