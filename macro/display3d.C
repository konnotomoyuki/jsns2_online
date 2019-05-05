void display3d()
{
  using namespace JSNS2;
  gStyle->SetPalette(1);

 TEveManager* eve = TEveManager::Create();

  TGeoManager *geom = new TGeoManager("ellipsoid", "");
  geom->SetNsegments(10);

  IDDisplay3D* display = new IDDisplay3D("h_PMTmap_charge", "PMT charge map", "Charge [ADC sum]");
  display->SetEveManager(eve);
  display->SetGeoManager(geom);

  const int nPMTs = display->GetNPMTs();
  for (int i = 0; i < nPMTs; i++) {
    display->SetBinContent(i+1, i);
  }
  display->Draw();

  TGeoNode* node = geom->GetTopNode();
  TEveGeoTopNode* en = new TEveGeoTopNode(geom, node);
  //en->SetVisLevel(4);
  //en->GetNode()->GetVolume()->SetVisibility(kFALSE);
  gEve->AddGlobalElement(en);
  gEve->Redraw3D(kTRUE);
}
