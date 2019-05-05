/*
  usage : root -l <rootfile with tree> read.C
*/

using namespace JSNS2;

void read() {
  TTree* tree = (TTree*)_file0->Get("tree");
  RawEvent* ev = new RawEvent();
  tree->SetBranchAddress("JSNS2::RawData::Event", &ev);
  TH1* h = new TH1D("h", "", 256, 0, 512);
  int count = 0;
  for (int iev = 0; iev < 10/*tree->GetEntries()*/; iev++) {
    tree->GetEntry(iev);
    std::vector<FADC>& fadcs(ev->GetFADCs());
    for (size_t ifadc = 0; ifadc < fadcs.size(); ifadc++) {
      FADC& fadc(fadcs[ifadc]);
      const std::vector<UChar_t>& samples(fadc.GetSamples(0));
      for (size_t i = 0; i < samples.size(); i++) {
	h->Fill(i*2, samples[i]);
      }
      count++;
    }
  }
  h->Scale(1./count);
  h->Draw("hist");
}
