#include "IDDisplay3D.hh"

#include "DBObject.hh"
#include "PMTGeoMapping.hh"

#include <TMath.h>
#include <TStyle.h>
#include <TGeometry.h>
#include <TGeoMaterial.h>
#include <TGeoMedium.h>
#include <TGeoSphere.h>
#include <TGeoScaledShape.h>
#include <TEveGeoNode.h>
#include <TGLViewer.h>
#include <TEveScene.h>

using namespace JSNS2;

IDDisplay3D::IDDisplay3D(const char* name, const char* title, const char* unit)
  : IDDisplay()
{
}

IDDisplay3D::~IDDisplay3D()
{
  delete m_PMTs;
}

void IDDisplay3D::SetBinContent(Int_t i, Double_t v)
{

}

Double_t IDDisplay3D::GetBinContent(Int_t i)
{
  return 0;
}

void IDDisplay3D::Draw(Option_t* opt)
{
  //m_geom = new TGeoManager("ellipsoid", "");
  m_geom->SetNsegments(20);
  //--- define some materials
  TGeoMaterial *matAl = new TGeoMaterial("Al", 26.98,13,2.7);
  TGeoMaterial *matAcr = new TGeoMaterial("Acryl", 10,13,2.7);

  //--- define some media
  TGeoMedium *Al = new TGeoMedium("Root Material",2, matAl);
  TGeoMedium *Clinder = new TGeoMedium("Root Material",3, matAcr);

  TGeoSphere *sphere = new TGeoSphere("sph", 1.0, g_rPMT, 0, 180., 90, 270);
  TGeoScaledShape *scaled = new TGeoScaledShape("ell", sphere, new TGeoScale(0.5,1,1));

  TGeoVolume *tank_barrel = m_geom->MakeTube("Tank_barrel", Clinder, g_radius, g_radius, g_height);
  TGeoVolume *tank_top = m_geom->MakeTube("Tank_top", Clinder, 0, g_radius, 0);
  TGeoVolume *tank_bottom = m_geom->MakeTube("Tank_top", Clinder, 0, g_radius, 0);
  tank_top->SetFillColor(kBlue);
  tank_top->SetLineColor(kWhite);
  tank_top->SetTransparency(60);
  tank_bottom->SetFillColor(kBlue);
  tank_bottom->SetLineColor(kWhite);
  //tank_bottom->SetLineColor(kBlue);
  tank_bottom->SetTransparency(60);
  tank_barrel->SetFillColor(kBlue);
  tank_barrel->SetLineColor(kWhite);
  //tank_barrel->SetLineColor(kBlue);
  tank_barrel->SetTransparency(60);
  
  TGeoVolume *tank = new TGeoVolumeAssembly("Tank");
  tank->AddNode(tank_top, 1, new TGeoCombiTrans(0, 0, g_height, NULL));
  tank->AddNode(tank_bottom, 2, new TGeoCombiTrans(0, 0, -g_height, NULL));
  tank->AddNode(tank_barrel, 3, new TGeoCombiTrans(0, 0, 0, NULL));

  m_PMTs = new TGeoVolumeAssembly("Triangular Holder");

  DBObject<DB::PMTGeoMapping> mapping("PMTGeoMapping");
  m_nPMTs = 0;
  for (auto& ip : mapping->GetPMTs()) {
    DB::PMTGeo& pmt(ip.second);
    int id = pmt.GetId();
    int location = pmt.GetLocation();
    double r = pmt.GetR();
    double phi = pmt.GetPhi();
    double z = pmt.GetZ();
    double x = 0, y = 0;
    TGeoVolume *roundPMT = new TGeoVolume(Form("PMT_%d", id), scaled, Al);
    roundPMT->SetLineColor(kYellow);
    roundPMT->SetTransparency(10);
    TGeoCombiTrans* ct = new TGeoCombiTrans(0, 0, 0, NULL);
    if (location == 0) { // top
      x = r * TMath::Sin(phi);
      y = r * TMath::Cos(phi);
      ct->RotateY(-90);
      ct->SetDx(x);
      ct->SetDy(y);
      ct->SetDz(z);
    } else if (location == 1) { // barrel
      ct->RotateZ(phi / TMath::Pi() * 180);
      ct->SetDx(r * TMath::Cos(phi));
      ct->SetDy(r * TMath::Sin(phi));
      ct->SetDz(z);
    } else if (location == 2) { // bottom
      x = r * TMath::Sin(phi);
      y = r * TMath::Cos(phi);
      ct->RotateY(90);
      ct->SetDx(x);
      ct->SetDy(y);
      ct->SetDz(z);
    }
    m_PMTs->AddNode(roundPMT, id+1, ct);
    m_nPMTs++;
  }
  TGeoVolume *Detector = new TGeoVolumeAssembly("Detector");
  Detector->AddNode(tank, 1, new TGeoRotation("r1", 0, -90, 0));
  Detector->AddNode(m_PMTs, 2, new TGeoRotation("r2", 0, -90, 0));
  m_geom->SetTopVolume(Detector);
  m_geom->CloseGeometry();
  /*
  TGeoNode* node = m_geom->GetTopNode();
  TEveGeoTopNode* en = new TEveGeoTopNode(m_geom, node);
  m_eve->AddGlobalElement(en);
  m_eve->Redraw3D(kTRUE);
  */
}

void IDDisplay3D::Update(TH1* h)
{
  Double_t max = h->GetMaximum();
  Double_t min = h->GetMinimum();
  Double_t delta = (max - min) / TColor::GetNumberOfColors();
  int nPMTs = GetNPMTs();
  for (int i = 0; i < nPMTs; i++) {
    Double_t data = h->GetBinContent(i+1);
    TGeoVolume* vol = m_PMTs->GetNode(i)->GetVolume();
    if (data < min) {
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
}
