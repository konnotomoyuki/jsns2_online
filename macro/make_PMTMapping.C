{
  using namespace JSNS2::DB;
  
  TFile* file = new TFile(Form("%s/db/PMTGeoMapping.root", getenv("JSNS2_HOME")), "recreate");
  std::ifstream fin(Form("%s/db/PMTMap_FullPMT_JSNS2.dat", getenv("JSNS2_HOME")));
  PMTGeoMapping* mapping = new PMTGeoMapping("PMTGeoMapping");
  int id;
  double r, phi, z;
  while (fin >> id >> r >> phi >> z) {
    int location = 0;// 0 top, 1 barrel, 2 bottom
    if (id < 36) { // top
      location = 0;
    } else if (id < 156) { // barrel
      location = 1;
    } else { // bottom
      location = 2;
    }
    mapping->Add(PMTGeo(id, location, r, phi, z));
  }
  mapping->Write();
  file->Close();
}
