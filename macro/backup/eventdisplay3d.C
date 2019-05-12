std::vector<TVector3> PMTpos;
const double Radius  = 1849.04 ;     //Radius of the acrylic tank
const double Height = 1470;     //Height of the acrylic tank
const double PMTRadius = 101.0 * 1.2;       //radius of the PMT

class MyTimer : public TTimer
 {

 private:
   TGeoVolume *PMTarray;
   int m_nPMTs;

 public:
   MyTimer(TGeoVolume *vol, int nPMTs)
     : TTimer(1000), PMTarray(vol), m_nPMTs(nPMTs)
   {
   }
 
   virtual Bool_t Notify()
   {
     // stop timer
     TurnOff();
     gStyle->SetPalette(55);
     Double_t max = 0;
     Double_t min = 0;
     Double_t x, y, z;
     while (true) {
       z = (gRandom->Rndm() - 0.5) * Height * 2;
       x = (gRandom->Rndm() - 0.5) * 2 * Radius;
       y = (gRandom->Rndm() - 0.5) * 2 * Radius;
       if (x*x+y*y<Radius*Radius) break;
     }
     TVector3 vtx(x,y,z);
     Double_t charge = gRandom->Rndm();
     for (int i = 0; i < m_nPMTs; i++) {
       TVector3& pos (PMTpos[i]);
       Double_t d = (pos - vtx).Mag();
       Double_t data = charge * 1000*10000./d;//gRandom->Gaus(0,70);
       if (max < data) max = data;
     }
     Double_t delta = (max - min) / TColor::GetNumberOfColors();
     for (int i = 0; i < m_nPMTs; i++) {
       TVector3& pos (PMTpos[i]);
       Double_t d = (pos - vtx).Mag();
       Double_t data = charge*1000*10000./d;//gRandom->Gaus(0,70);
       if (data < (max - min) * 0.2 + min) data = -1;
       TGeoVolume* vol = PMTarray->GetNode(i)->GetVolume();
       if (data <= min) {
	 vol->SetVisibility(false);
       } else{
	 Color_t color =TColor::GetColorPalette(TColor::GetNumberOfColors() - 1);
	 for ( Int_t c = 0; c < TColor::GetNumberOfColors(); c++ ) {
	   if ( data <= min + c * delta ) {
	     color = TColor::GetColorPalette(c);
	     break;
	   }
	 }
	 vol->SetVisibility(true);
	 vol->SetLineColor(color);
       }
     }
     gEve->GetDefaultGLViewer()->RequestDraw();
     gEve->DoRedraw3D();
     gEve->GetGlobalScene()->Changed();

     // restart timer
     SetTime(1000);
     Reset();
     TurnOn();
     return true;
   }

 };

void eventdisplay3d() 
{

  const double barrelX = 0.5;
  const double barrelY = 0.5;
  const double barrelX1 = 0.5-TMath::Pi()*Radius;
  const double barrelY1 = 0.5-Height;
  const double barrelX2 = 0.5+TMath::Pi()*Radius;
  const double barrelY2 = 0.5+Height;
  
  const double topX = 0;
  const double topY = 0;
  
  const double bottomX = 0;
  const double bottomY = 0;

  TEveManager::Create();
  
  TGeoManager *geom = new TGeoManager("cone", "poza4");
  geom->SetNsegments(20);
  //--- define some materials
  TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
  TGeoMaterial *matAl = new TGeoMaterial("Al", 26.98,13,2.7);
  TGeoMaterial *matAcr = new TGeoMaterial("Acryl", 10,13,2.7);

  //--- define some media
  TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);
  TGeoMedium *Al = new TGeoMedium("Root Material",2, matAl);
  TGeoMedium *Clinder = new TGeoMedium("Root Material",3, matAcr);

  TGeoSphere *sphere = new TGeoSphere("sph", 1.0, PMTRadius, 0, 180., 0, 360);
  TGeoScaledShape *scaled = new TGeoScaledShape("ell", sphere, new TGeoScale(0.5,1,1));

  TGeoVolume *tank_barrel = geom->MakeTube("Tank_barrel", Clinder, Radius, Radius, Height);
  TGeoVolume *tank_top = geom->MakeTube("Tank_top", Clinder, 0, Radius, 0);
  TGeoVolume *tank_bottom = geom->MakeTube("Tank_top", Clinder, 0, Radius, 0);
  tank_top->SetLineColor(kBlue);
  tank_top->SetTransparency(50);
  tank_bottom->SetLineColor(kBlue);
  tank_bottom->SetTransparency(50);
  tank_barrel->SetLineColor(kBlue);
  tank_barrel->SetLineWidth(2);
  tank_barrel->SetTransparency(50);
  
  TGeoVolume *tank = new TGeoVolumeAssembly("Tank");
  tank->AddNode(tank_top, 1, new TGeoCombiTrans(0, 0, Height, NULL));
  tank->AddNode(tank_bottom, 2, new TGeoCombiTrans(0, 0, -Height, NULL));
  tank->AddNode(tank_barrel, 3, new TGeoCombiTrans(0, 0, 0, NULL));

  TGeoVolume *PMTarray = new TGeoVolumeAssembly("Triangular Holder");
  std::ifstream fin("PMTMap_FullPMT_JSNS2.dat");
  int id;
  double r, phi, z;
  int nPMTs = 0;
  while (fin >> id >> r >> phi >> z) {
    double x = 0, y = 0;
    TGeoVolume *roundPMT = new TGeoVolume("PMT", scaled, Al);
    roundPMT->SetLineColor(kYellow);
    //roundPMT->SetTransparency(10);
    TGeoCombiTrans* ct = new TGeoCombiTrans(0, 0, 0, NULL);
    if (id < 36) { // top
      x = topX + r * TMath::Cos(phi);
      y = topY - r * TMath::Sin(phi);
      ct->RotateY(-90);
      ct->SetDx(x);
      ct->SetDy(y);
      ct->SetDz(z);
    } else if (id < 156) { // barrel
      x = r * TMath::Cos(phi);
      y = r * TMath::Sin(phi);
      ct->RotateZ(phi / TMath::Pi() * 180);
      ct->SetDx(x);
      ct->SetDy(y);
      ct->SetDz(z);
    } else { // bottom
      x = bottomX + r * TMath::Cos(phi);
      y = bottomY + r * TMath::Sin(phi);
      ct->RotateY(90);
      ct->SetDx(x);
      ct->SetDy(y);
      ct->SetDz(z);
    }
    PMTpos.push_back(TVector3(x,y,z));
    PMTarray->AddNode(roundPMT, nPMTs+1, ct);
    nPMTs++;
  }
  TGeoVolume *Detector = new TGeoVolumeAssembly("Detector");
  Detector->AddNode(tank, 1, new TGeoRotation("r1", 0, -90, 0));
  Detector->AddNode(PMTarray, 2, new TGeoRotation("r2", 0, -90, 0));
  geom->SetTopVolume(Detector);

  //Detector->Draw("ogl");

  //--- close the geometry
  geom->CloseGeometry();
  
  TGeoNode* node = geom->GetTopNode();
  TEveGeoTopNode* en = new TEveGeoTopNode(geom, node);
  en->SetVisLevel(4);
  en->GetNode()->GetVolume()->SetVisibility(kFALSE);
  gEve->AddGlobalElement(en);
  gEve->Redraw3D(kTRUE);
  Double_t max = 200;
  Double_t min = 0;
  Double_t delta = (max - min) / TColor::GetNumberOfColors();
  for (int i = 0; i < nPMTs; i++) {
    Double_t data = i;
    TGeoVolume* vol = PMTarray->GetNode(i)->GetVolume();
    if (data < min) {
      vol->SetLineColor(kWhite);
    } else{
      Color_t color =TColor::GetColorPalette(TColor::GetNumberOfColors() - 1);
      for ( Int_t c = 0; c < TColor::GetNumberOfColors(); c++ ) {
        if ( data <= min + c * delta ) {
          color = TColor::GetColorPalette(c);
          break;
        }
      }
      vol->SetLineColor(color);
    }
  }
  //en->ExpandIntoListTreesRecursively();

  MyTimer* timer = new MyTimer(PMTarray, nPMTs);
  timer->SetTime(1000);
  timer->Reset();
  timer->TurnOn();
}
