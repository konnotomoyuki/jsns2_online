#include "DummyEventModule.hh"

#include "StoredObject.hh"
#include "EventMetaData.hh"
#include "RawEvent.hh"
#include "DBObject.hh"
#include "FADCMapping.hh"

#include <TMath.h>
#include <TRandom3.h>

#include <cstdlib>

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
  StoredObject<RawEvent> ev;
  ev->SetHeaderSize(3);
  ev->SetHeaderMagic(0xfa);
  ev->SetRunNumber(m_runNum);
  ev->SetTrailerMagic(0xcafebabe);
  ev->GetFADCs().clear();
  for (auto& ib : mapping->GetBoards()) {
    DB::FADCBoard& board(ib.second);
    FADC fadc;
    fadc.SetSerial(board.GetSerial());
    fadc.SetBoard(board.GetId());
    fadc.SetEventSize(256/4 * 8 + 4);
    fadc.SetChannelMask(0xFF);
    for (int i = 0; i < 8; i++) {
      fadc.GetSamples(i).resize(256);
    }
    ev->GetFADCs().push_back(fadc);
  }
  m_eventNum = 0;
  return true;
}

Bool_t DummyEventModule::ProcessEvent()
{
  m_time += gRandom->Exp(1./300.*1e9) * 16;
  StoredObject<EventMetaData> meta;
  meta->SetEventNumber(m_eventNum);
  StoredObject<RawEvent> ev;
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
  Double_t v[256];
  for (auto& fadc : ev->GetFADCs()) {
    fadc.SetEventCount(m_eventNum);
    fadc.SetTimeTag(m_time);
    DB::FADCBoard& board(mapping->GetBoard(fadc.GetSerial()));
    Double_t factor = 0;
    for (int ic = 0; ic < 8; ic++) {
      DB::FADCChannel& ch(board.GetChannel(ic));
      std::vector<UChar_t>& samples(fadc.GetSamples(ic));
      if (m_tree) {
	if (numHitsPMT->size() > (size_t)ch.GetPMT())
	  factor = 300 * numHitsPMT->at(ch.GetPMT());
	else 
	  factor = 0;
      } else {
	if (ic % 2 == 0) {
	  factor = 5000 * gRandom->Gaus(1.5, 2);
	}
	if (factor < 0) factor = 0;
      }
      for (size_t i = 0; i < samples.size(); i++) {
	v[i] = 244.5 + 2.*rand()/RAND_MAX - 1.;
      }
      int t = (factor> 0 &&m_tree)?timePMT->at(ch.GetPMT()):(2.*rand()/RAND_MAX);
      for (size_t i = 0; i < samples.size(); i++) {
	if (i+t < samples.size())
	  v[i+t] += m_f->Eval(i*2) * factor * (ic%2==0?0.2:1.0);
	if (v[i] < 0) v[i] = 0;
	samples[i] = (UShort_t)v[i];
      }
    }
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


