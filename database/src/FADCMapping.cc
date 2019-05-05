#include "FADCMapping.hh"

ClassImp(JSNS2::DB::FADCChannel);
ClassImp(JSNS2::DB::FADCBoard);
ClassImp(JSNS2::DB::FADCMapping);

using namespace JSNS2;
using namespace JSNS2::DB;

FADCMapping::FADCMapping()
  : TNamed() 
{

}

FADCMapping::FADCMapping(const char* name, const char* title)
  : TNamed(name, title)
{

}

FADCMapping::FADCMapping(const char* name)
  : TNamed(name, "FADC channel mapping to PMTs")
{

}

void FADCBoard::AddChannel(int id, FADCChannel& channel)
{
  m_channels.insert(std::pair<int, FADCChannel>(id, channel));
}
void FADCMapping::AddBoard(int serial, FADCBoard& board) { 
  m_boards.insert(std::pair<int, FADCBoard>(serial, board));
}

void FADCMapping::Print()
{
  for (auto& ib: GetBoards()) {
    FADCBoard& board(ib.second);
    int i = board.GetSerial();
    printf("# FADC #%d (serial=0x%x, board=0x%x) \n", i, board.GetSerial(), board.GetId());
    printf("FADC[0x%x].serial             : 0x%x\n", i, board.GetSerial());
    printf("FADC[0x%x].board              : 0x%x\n", i, board.GetId());
    for (auto& ic : board.GetChannels()) {  
      FADCChannel& channel(ic.second);
      int j = channel.GetId();
      printf("FADC[0x%x].channel[%d].masked  : %s\n", i, j, channel.IsMasked()?"true":"false");
      printf("FADC[0x%x].channel[%d].pmtid   : %d\n", i, j, channel.GetPMT());
      printf("FADC[0x%x].channel[%d].gain    : %s\n", i, j, channel.GetGain()?"high":"low");
    }
    printf("\n");
  }
}
