{
  using namespace JSNS2::DB;
  
  TFile* file = new TFile(Form("%s/db/FADCMapping.root", getenv("JSNS2_HOME")), "recreate");
  FADCMapping* obj = new FADCMapping("FADCMapping");

  int pmtid = 0;
  for (int i = 0; i < 200/4; i++) {
    FADCBoard board;
    int serial = 0x1000+0x4*i;
    board.SetSerial(serial);
    board.SetId(i);
    for (int j = 0; j < 4; j++) {  
      FADCChannel channel;
      channel.SetId(j*2);
      channel.SetMasked(false);
      channel.SetPMT(pmtid);
      channel.SetGain(false);
      board.AddChannel(j*2, channel);
      channel.SetId(j*2+1);
      channel.SetMasked(false);
      channel.SetPMT(pmtid);
      channel.SetGain(true);
      board.AddChannel(j*2+1, channel);
      pmtid++;
    }
    obj->AddBoard(serial, board);
    obj->Print();
  }
  obj->Write();
  file->Close();
}
