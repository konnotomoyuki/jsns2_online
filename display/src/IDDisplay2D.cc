#include "IDDisplay2D.hh"

#include "DBObject.hh"
#include "PMTGeoMapping.hh"

#include <TMath.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaletteAxis.h>
#include <TFrame.h>

using namespace JSNS2;

const double IDDisplay2D::g_barrelX  = 0.5;
const double IDDisplay2D::g_barrelY  = 0.5;
const double IDDisplay2D::g_barrelX1 = 0.5 - TMath::Pi() * g_radius / g_scale;
const double IDDisplay2D::g_barrelY1 = 0.5 - g_height / g_scale;
const double IDDisplay2D::g_barrelX2 = 0.5 + TMath::Pi() * g_radius / g_scale;
const double IDDisplay2D::g_barrelY2 = 0.5 + g_height / g_scale;
const double IDDisplay2D::g_topX     = 0.5;
const double IDDisplay2D::g_topY     = 0.5 + (g_height + g_radius) / g_scale;
const double IDDisplay2D::g_bottomX  = 0.5;
const double IDDisplay2D::g_bottomY  = 0.5 - (g_height + g_radius) / g_scale;

IDDisplay2D::IDDisplay2D(const char* name, const char* title, const char* unit)
  : IDDisplay()
{
  //const double height = g_height / g_scale;
  const double radius = g_radius / g_scale;
  const double rPMT = g_rPMT / g_scale;

  m_barrelBox = new TBox(g_barrelX1, g_barrelY1, g_barrelX2, g_barrelY2);
  m_barrelBox->SetLineWidth(1);
  m_barrelBox->SetLineColor(kBlack);
  m_barrelBox->SetFillColor(kBlack);
  
  m_topCircle = new TEllipse(g_topX, g_topY, radius, radius);
  m_topCircle->SetLineWidth(1);
  m_topCircle->SetLineColor(kBlack);
  m_topCircle->SetFillColor(kBlack);
  
  m_bottomCircle = new TEllipse(g_bottomX, g_bottomY, radius, radius);
  m_bottomCircle->SetLineWidth(1);
  m_bottomCircle->SetLineColor(kBlack);
  m_bottomCircle->SetFillColor(kBlack);

  m_PMTs = new TH2Poly();
  m_PMTs->SetName(name);
  m_PMTs->SetTitle(Form("%s;%s;", title, unit));

  int numPoints = 100;
  double binX[100];
  double binY[100];
  binX[0] = g_barrelX1; binY[0] = g_barrelY1;
  binX[1] = g_barrelX2; binY[1] = g_barrelY1;
  binX[2] = g_barrelX2; binY[2] = g_barrelY2;
  binX[3] = g_barrelX1; binY[3] = g_barrelY2;
  binX[4] = g_barrelX1; binY[4] = g_barrelY1;
  m_PMTs->AddBin(5, binX, binY);
  
  for (int i = 0; i < numPoints; ++i) {
    double pphi = i * 2.0 * TMath::Pi() / (numPoints - 1);
    binX[i] = g_topX + radius * TMath::Sin(pphi);
    binY[i] = g_topY + radius * TMath::Cos(pphi);
  }
  m_PMTs->AddBin(numPoints, binX, binY);

  for (int i = 0; i < numPoints; ++i) {
    double pphi = i * 2.0 * TMath::Pi() / (numPoints - 1);
    binX[i] = g_bottomX + radius * TMath::Sin(pphi);
    binY[i] = g_bottomY + radius * TMath::Cos(pphi);
  }
  m_PMTs->AddBin(numPoints, binX, binY);
  m_PMTs->SetBinContent(1, -9);
  m_PMTs->SetBinContent(2, -9);
  m_PMTs->SetBinContent(3, -9);
  m_PMTs->SetMinimum(-20);
  m_PMTs->SetLineWidth(2);
  m_PMTs->SetLineColor(kWhite);

  m_nPMTs = 0;
  DBObject<DB::PMTGeoMapping> mapping("PMTGeoMapping");
  for (auto& ip : mapping->GetPMTs()) {
    DB::PMTGeo& pmt(ip.second);
    //int id = pmt.id;
    int location = pmt.GetLocation();
    double r = pmt.GetR() / g_scale;
    double phi = pmt.GetPhi();
    double z = pmt.GetZ() / g_scale;
    double x = 0, y = 0;
    if (location == 0) { // top
      x = g_topX + r * TMath::Sin(phi);
      y = g_topY - r * TMath::Cos(phi);
    } else if (location == 1) { // barrel
      x = g_barrelX + r * phi;
      y = g_barrelY + z;
    } else if (location == 2) { // bottom
      x = g_bottomX + r * TMath::Sin(phi);
      y = g_bottomY + r * TMath::Cos(phi);
    }
    for (int i = 0; i < numPoints; ++i) {
      double pphi = i * 2.0*TMath::Pi() / (numPoints - 1);
      binX[i] = x + rPMT * TMath::Sin(pphi);
      binY[i] = y + rPMT * TMath::Cos(pphi);
    }
    m_PMTs->AddBin(numPoints, binX, binY);
    //m_plPMTs[m_nPMTs] = new TEllipse(x, y, rPMT, rPMT);
    //m_plPMTs[m_nPMTs]->SetFillStyle(0);
    //m_plPMTs[m_nPMTs]->SetLineColor(kBlack);
    m_nPMTs++;
  }
  m_title = new TText(0.01, 0.95, title);
  m_title->SetTextColor(kBlack);
  m_title->SetTextSize(0.05);

  m_unit = new TText(0.75, g_barrelY1-0.05, unit);
  m_unit->SetTextColor(kBlack);
  m_unit->SetTextSize(0.03);
}

IDDisplay2D::~IDDisplay2D()
{
  delete m_PMTs;
  delete m_title;
  delete m_unit;
  delete m_topCircle;
  delete m_bottomCircle;
  delete m_title;
  delete m_unit;
  /*
  for (int i = 0; i < m_nPMTs; i++) {
    delete m_plPMTs[i];
  }
  */
}

void IDDisplay2D::Draw(Option_t* opt)
{
  gPad->Clear();

  m_PMTs->SetMinimum(-10);
  Double_t v = (m_PMTs->GetMaximum() - m_PMTs->GetMinimum())*0.0001 + m_PMTs->GetMinimum();
  m_PMTs->SetBinContent(1, v);
  m_PMTs->SetBinContent(2, v);
  m_PMTs->SetBinContent(3, v);
  m_PMTs->GetXaxis()->SetAxisColor(0);
  m_PMTs->GetYaxis()->SetAxisColor(0);
  m_PMTs->SetTickLength(0, "X");
  m_PMTs->SetTickLength(0, "Y");
  m_PMTs->SetMinimum(-2);
  m_PMTs->Draw("colz");
  m_title->Draw("SAME");
  m_unit->Draw("SAME");
  /*
  Double_t max = m_PMTs->GetMaximum();
  Double_t min = m_PMTs->GetMinimum();
  Double_t delta = (max - min) / TColor::GetNumberOfColors();
  DBObject<DB::PMTGeoMapping> mapping();
  for (auto& ip : mapping->GetPMTs()) {
    DB::PMTGeo& pmt(ip.second);
    Double_t data = m_PMTs->GetBinContent(pmt.GetId());
    if (data < min) {
      m_plPMTs[i]->SetFillColor(kWhite);
    } else{
      m_plPMTs[i]->SetFillColor(TColor::GetColorPalette(TColor::GetNumberOfColors() - 1));
      for ( Int_t c = 0; c < TColor::GetNumberOfColors(); c++ ) {
        if ( data <= min + c * delta ) {
          m_plPMTs[i]->SetFillColor(TColor::GetColorPalette(c));
          break;
        }
      }
    }
    m_plPMTs[i]->Draw("same");
  }
  */

  TPaletteAxis *palette = (TPaletteAxis*)m_PMTs->GetListOfFunctions()->FindObject("palette");
  if (palette) {
    palette->SetX1NDC(0.75);
    palette->SetX2NDC(0.8);
    palette->SetY1NDC(0.02);
    palette->SetY2NDC(g_barrelY1 - 0.06);
  }
}

