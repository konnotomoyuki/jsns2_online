#ifndef _JSNS2_FADCMapping_h
#define _JSNS2_FADCMapping_h

#include <TNamed.h>

#include <map>
#include <vector>

namespace JSNS2 {
 
  namespace DB {

   class FADCChannel : public TObject {

    public:
      FADCChannel() {}
      virtual ~FADCChannel() {}

    public:
      Int_t GetId() const { return m_id; }
      void SetId(Int_t id) { m_id = id; }
      Bool_t IsMasked() const { return m_masked; }
      void SetMasked(Bool_t masked) { m_masked = masked; }
      Int_t GetPMT() const { return m_pmt; }
      void SetPMT(Int_t pmt) { m_pmt = pmt; }
      Bool_t GetGain() const { return m_gain; }
      void SetGain(Bool_t gain) { m_gain = gain; }

    private:
      Int_t m_id;
      Bool_t m_masked;
      Int_t m_pmt;
      Bool_t m_gain;

    public:
      ClassDef(FADCChannel, 1);

    };

    class FADCBoard : public TObject {

    public:
      FADCBoard() {}
      virtual ~FADCBoard() {}

    public:
      Int_t GetSerial() const { return m_serial; }
      void SetSerial(Int_t serial) { m_serial = serial; }
      Int_t GetId() const { return m_board; }
      void SetId(Int_t board) { m_board = board; }
      void AddChannel(int id, FADCChannel& channel);
      FADCChannel& GetChannel(int id) { return m_channels.at(id); }
      const FADCChannel& GetChannel(int id) const { return m_channels.at(id); }
      std::map<int, FADCChannel>& GetChannels() { return m_channels; }
      const std::map<int, FADCChannel>& GetChannels() const { return m_channels; }

    private:
      Int_t m_serial;
      Int_t m_board;
      std::map<int, FADCChannel> m_channels;

    public:
      ClassDef(FADCBoard, 1);

    };

    class FADCMapping : public TNamed {

    public:
      FADCMapping();
      FADCMapping(const char* name);
      FADCMapping(const char* name, const char* title);
      virtual ~FADCMapping() {
      }

    public:
      Bool_t HasBoard(int serial) const { return m_boards.find(serial) != m_boards.end(); }
      std::map<int, FADCBoard>& GetBoards() { return m_boards; }
      const std::map<int, FADCBoard>& GetBoards() const { return m_boards; }
      FADCBoard& GetBoard(int serial) { return m_boards.at(serial); }
      const FADCBoard& GetBoard(int serial) const { return m_boards.at(serial); }
      void AddBoard(int serial, FADCBoard& board);

      virtual void Print();

    private:
      std::map<int, FADCBoard> m_boards;

    public:
      ClassDef(FADCMapping, 1); 

    };

  }

}

#endif  
