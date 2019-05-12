#include "DummyEventModule.hh"

#include "StoredObject.hh"
#include "EventMetaData.hh"
#include "RawEvent.hh"
#include "RawFADCArray.hh"
#include "DBObject.hh"
#include "FADCMapping.hh"
#include "PMTGeoMapping.hh"

#include <TMath.h>
#include <TRandom3.h>
#include <TVector3.h>

#include <cstdlib>
#include <iostream>

using namespace JSNS2;

Double_t myfunction(Double_t *x, Double_t *par)
{
  Double_t sigma = par[2] * (x[0] - par[0]) + par[3];
  Double_t f = par[1] * TMath::Gaus(x[0], par[0], sigma, kTRUE);
  return f;
}


DummyEventModule::DummyEventModule() : Module ("DummyEvent")
{
  m_filepath = "";
  m_tree = NULL;
}

DummyEventModule::~DummyEventModule()
{

}

int eventNum;
std::vector<int> *indexPMT = 0;
std::vector<int> *numHitsPMT = 0;
std::vector<double> *timePMT = 0;

Bool_t DummyEventModule::Initialize()
{
  StoredObject<EventMetaData>::Create();
  StoredObject<RawEvent>::Create();
  StoredObject<RawFADCArray>::Create();
  if (m_filepath.size() > 0)  {
    m_file = new TFile(m_filepath.c_str());
    m_tree = (TTree*)m_file->Get("MCData");
    m_tree->SetBranchAddress("EventNum", &eventNum);
    m_tree->SetBranchAddress("IndexPMT", &indexPMT);
    m_tree->SetBranchAddress("NumHitsPMT", &numHitsPMT);
    m_tree->SetBranchAddress("TimePMT", &timePMT);
  } 
  m_f = new TF1("myfunc",myfunction, 0, 500,4);
  m_f->SetParameters(100, -1, 0.2, 20.8);
  return true;
}

Bool_t DummyEventModule::BeginRun()
{
  StoredObject<EventMetaData> meta;
  meta->SetRunNumber(m_runNum);
  meta->SetRunType("dummy");
  DBObject<DB::FADCMapping> mapping("FADCMapping");
  DBObject<DB::PMTGeoMapping> pmtgeo("PMTGeoMapping");
  m_eventNum = 0;
  return true;
}

Bool_t DummyEventModule::ProcessEvent()
{
  m_time += gRandom->Exp(1./300.*1e9) * 16;
  StoredObject<EventMetaData> meta;
  meta->SetEventNumber(m_eventNum);
  StoredObject<RawEvent> ev;
  ev->Reset();
  ev->SetHeaderSize(sizeof(RawEventHeader) / sizeof(int));
  ev->SetHeaderMagic(0xfa);
  ev->SetTrailerMagic(0xcafebabe);
  if (m_eventNum > 100000) {
    m_eventNum = 0;
    m_runNum++;
    meta->SetRunNumber(m_runNum);
    ev->SetRunNumber(m_runNum);
  }
  if (m_tree) {
    m_tree->GetEntry(m_eventNum % (int)m_tree->GetEntries());
  }
  DBObject<DB::FADCMapping> mapping;
  DBObject<DB::PMTGeoMapping> pmtgeo;
  Double_t v[256];
  UInt_t buf[1024];
  StoredObject<RawFADCArray> fadcs;
  fadcs->Reset();
  const double Radius  = 1849.04 ;     //Radius of the acrylic tank
  const double Height = 1470;     //Height of the acrylic tank
  Double_t x, y, z;
  while (true) {
    z = (gRandom->Rndm() - 0.5) * Height * 2;
    x = (gRandom->Rndm() - 0.5) * 2 * Radius;
    y = (gRandom->Rndm() - 0.5) * 2 * Radius;
    if (x*x+y*y<Radius*Radius) break;
  }
  TVector3 vtx(x,y,z);
  Double_t charge = gRandom->Rndm();

  for (auto& ib : mapping->GetBoards()) {
    DB::FADCBoard& board(ib.second);
    RawFADC fadc;
    fadc.SetSerial(board.GetSerial());
    fadc.SetBoard(board.GetId());
    fadc.SetChannelMask(0xFF);
    fadc.SetEventSize(8 * 248 / 4 + 4);
    fadc.SetEventCount(m_eventNum);
    fadc.SetTimeTag(m_time);
    Double_t factor = 0;
    for (int ic = 0; ic < 8; ic++) {
      DB::FADCChannel& ch(board.GetChannel(ic));
      std::vector<UChar_t>& samples(fadc[ic]);
      samples.resize(248);
      if (m_tree) {
	if (numHitsPMT->size() > (size_t)ch.GetPMT())
	  factor = 300 * numHitsPMT->at(ch.GetPMT());
	else 
	  factor = 0;
      } else {
	const DB::PMTGeo& pmt(pmtgeo->Get(ch.GetPMT()));
	double r = pmt.GetR();
	double phi = pmt.GetPhi();
	double z = pmt.GetZ();
	double x = r * TMath::Cos(phi);
	double y = r * TMath::Sin(phi);
	TVector3 pos (x,y,z);
	Double_t d = (pos - vtx).Mag();
	factor = charge * 1000*10000./d;
	if (factor < 100) factor = 0;
      }
      for (size_t i = 0; i < samples.size(); i++) {
	v[i] = 244.5 + 2.*rand()/RAND_MAX - 1.;
      }
      int t = (factor> 0 &&m_tree)?timePMT->at(ch.GetPMT()):(2.*rand()/RAND_MAX);
      for (size_t i = 0; i < samples.size(); i++) {
	if (i+t < samples.size())
	  v[i+t] += m_f->Eval(i*2) * factor * (ch.GetGain()%2==0?0.2:1.0);
	if (v[i] < 0) v[i] = 0;
	samples[i] = (UChar_t)v[i];
      }
    }
    int nword = fadc.Write(buf);
    ev->Add(RawDataBlock(nword, buf));
    fadcs->Add(fadc);
  }
  m_eventNum++;
  return true;
}

Bool_t DummyEventModule::EndRun()
{
  return true;
}

Bool_t DummyEventModule::Finalize()
{
  return true;
}


