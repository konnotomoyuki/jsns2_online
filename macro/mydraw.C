class Display2D {
  
public:
  Display2D(const char* name, const char* title, const char* unit);
  virtual ~Display2D()
  {
    delete m_PMTs;
    delete m_title;
    delete m_unit;
  }
  
public:
  Int_t GetNPMTs() const { return m_nPMTs; }
  void SetBinContent(Int_t i, Double_t v) { m_PMTs->SetBinContent(i+1, v); }
  Double_t GetBinContent(Int_t i) { return m_PMTs->GetBinContent(i+1); }
  void Draw(Option_t* opt="colz");
  
private:
  Int_t m_nPMTs;
  TH2Poly* m_PMTs;
  TBox* m_barrelBox;
  TEllipse* m_topCircle;
  TEllipse* m_bottomCircle;
  TText* m_title;
  TText* m_unit;

};

Display2D::Display2D(const char* name, const char* title, const char* unit)
{
  //Some constants which define the geometry (in mm)
  double Radius  = 1849.04 ;     //Radius of the acrylic tank
  double Height = 1470;     //Height of the acrylic tank
  double PMTRadius = 101.0 * 1.5;       //radius of the PMT
  const double range = 12000;
  Radius /= range;
  Height /= range;
  PMTRadius /= range;
  
  const double barrelX = 0.5;
  const double barrelY = 0.5;
  const double barrelX1 = 0.5-TMath::Pi()*Radius;
  const double barrelY1 = 0.5-Height;
  const double barrelX2 = 0.5+TMath::Pi()*Radius;
  const double barrelY2 = 0.5+Height;

  const double topX = 0.5;
  const double topY = 0.5 + Height + Radius;

  const double bottomX = 0.5;
  const double bottomY = 0.5 - Height - Radius;

  m_barrelBox = new TBox(barrelX1, barrelY1, barrelX2, barrelY2);
  m_barrelBox->SetLineWidth(1);
  m_barrelBox->SetLineColor(kGray+3);
  m_barrelBox->SetFillColor(kGray);
  
  m_topCircle = new TEllipse(topX, topY, Radius, Radius);
  m_topCircle->SetLineWidth(1);
  m_topCircle->SetLineColor(kGray+3);
  m_topCircle->SetFillColor(kGray);
  
  m_bottomCircle = new TEllipse(bottomX, bottomY, Radius, Radius);
  m_bottomCircle->SetLineWidth(1);
  m_bottomCircle->SetLineColor(kGray+3);
  m_bottomCircle->SetFillColor(kGray);

  m_PMTs = new TH2Poly();
  m_PMTs->SetName(name);
  m_PMTs->SetTitle(Form("%s;%s;", title, unit));

  std::ifstream fin("PMTMap_FullPMT_JSNS2.dat");
  int id;
  double r, phi, z;
  m_nPMTs = 0;
  for (int i = 0; i < 8; i++) {

  while (fin >> id >> r >> phi >> z) {
    r /= range;
    z /= range;
    double x = 0, y = 0;
    if (id < 36) { // top
      x = topX + r * TMath::Sin(phi);
      y = topY - r * TMath::Cos(phi);
    } else if (id < 156) { // barrel
      x = barrelX + r * phi;
      y = barrelY + z;
    } else { // bottom
      x = bottomX + r * TMath::Sin(phi);
      y = bottomY + r * TMath::Cos(phi);
    }
    int numPoints = 100;
    double binX[100];
    double binY[100];
    for (int i = 0; i < numPoints; ++i) {
      double pphi = i * 2.0*TMath::Pi() / (numPoints - 1);
      binX[i] = x + PMTRadius * TMath::Sin(pphi);
      binY[i] = y + PMTRadius * TMath::Cos(pphi);
    }
    m_PMTs->AddBin(numPoints, binX, binY);
    m_nPMTs++;
  }

  m_title = new TText(0.01, 0.95, title);
  m_title->SetTextColor(kBlack);
  m_title->SetTextSize(0.05);

  m_unit = new TText(0.75, barrelY1-0.05, unit);
  m_unit->SetTextColor(kBlack);
  m_unit->SetTextSize(0.03);
}

void Display2D::Draw(Option_t* opt)
{
  //Some constants which define the geometry (in mm)
  double Radius  = 1849.04 ;     //Radius of the acrylic tank
  double Height = 1470;     //Height of the acrylic tank
  double PMTRadius = 101.0 * 1.5;       //radius of the PMT
  const double range = 12000;
  Radius /= range;
  Height /= range;
  PMTRadius /= range;
  
  const double barrelX = 0.5;
  const double barrelY = 0.5;
  const double barrelX1 = 0.5-TMath::Pi()*Radius;
  const double barrelY1 = 0.5-Height;
  const double barrelX2 = 0.5+TMath::Pi()*Radius;
  const double barrelY2 = 0.5+Height;

  const double topX = 0.5;
  const double topY = 0.5 + Height + Radius;

  const double bottomX = 0.5;
  const double bottomY = 0.5 - Height - Radius;

  gPad->Clear();
  m_barrelBox->Draw("L");
  m_topCircle->Draw("SAME");
  m_bottomCircle->Draw("SAME");
  m_PMTs->Draw(Form("%s SAME", opt));

  TPaletteAxis *palette = (TPaletteAxis*)m_PMTs->GetListOfFunctions()->FindObject("palette");
  if (palette) {
    palette->SetX1NDC(0.75);
    palette->SetX2NDC(0.8);
    palette->SetY1NDC(0.06);
    palette->SetY2NDC(barrelY1 - 0.02);
  }

  m_title->Draw("same");
  m_unit->Draw("same");
}

void mydraw()
{
  gStyle->SetPalette(1);

  THttpServer *http = new THttpServer("http:8080");
  http->ProcessRequests();

  Display2D* display = new Display2D("h_PMTmap_charge", "PMT charge map", "Charge [ADC sum]"); 
  const int nPMTs = display->GetNPMTs();
  for (int i = 0; i < nPMTs; i++) {
    display->SetBinContent(i+1, i);
  }
  TCanvas* c = new TCanvas("c", "c", 640, 640);
  c->cd();
  display->Draw();

  http->Register(Form("/%s", "C"), c);
  while (true) {
    for (int i = 0; i < nPMTs; i++) {
      display->SetBinContent(i, gRandom->Gaus(100,10));
    }
    c->cd();
    display->Draw();
    c->Update();
    gSystem->ProcessEvents();
    sleep(1);
    gSystem->ProcessEvents();
  }
}

