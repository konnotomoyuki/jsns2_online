void convert(const char* filepath) {
  int fd = ::open(filepath, O_RDONLY, 0666);
  UInt_t* buf = new UInt_t[1024*1024*5];//20MB
  TFile file ("data.root", "recreate");
  TTree* tree = new TTree("RawDataTree", "RawDataTree");
  JSNS2::RawData::Event* ev = new JSNS2::RawData::Event();
  tree->Branch("Waveform","JSNS2::RawData::Event", ev);
  for (int iev = 0; ; iev++) {
    UInt_t* p = buf;
    int ret = read(fd, p, sizeof(int));
    if (ret <= 0) break;
    p++;
    UInt_t hsize = (0xFFFF & buf[0]);
    ret = read(fd, p, (hsize - 1) * sizeof(int));
    if (ret <= 0) break;
    p += ret / sizeof(int);
    UInt_t nboards = buf[2];
    for (unsigned int i = 0; i < nboards; i++) {
      unsigned int serial;
      ret = read(fd, p, sizeof(int));
      if (ret <= 0) break;
      p++;
      ret = read(fd, p, sizeof(int));
      if (ret <= 0) break;
      int nword = *p & 0x0FFFFFFF;
      p++;
      ret = read(fd, p, (nword-1) * sizeof(int));
      if (ret <= 0) break;
      p += ret / sizeof(int);
    }
    ret = read(fd, p, sizeof(int));
    if (ret <= 0) break;
    ev->Read(buf);
    tree->Fill();
  }
  tree->Write();
  file.Close();
}
