void display2d()
{
  using namespace JSNS2;
  gStyle->SetPalette(1);

  IDDisplay2D* display = new IDDisplay2D("h_PMTmap_charge", "PMT charge map", "Charge [ADC sum]");
  const int nPMTs = display->GetNPMTs();
  for (int i = 0; i < nPMTs; i++) {
    display->SetBinContent(i+1, i);
  }
  TCanvas* c = new TCanvas("c", "c", 640, 640);
  display->Draw();
}
