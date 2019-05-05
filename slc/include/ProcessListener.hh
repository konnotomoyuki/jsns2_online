#ifndef _JSNS2_ProcessListener_hh
#define _JSNS2_ProcessListener_hh

namespace JSNS2 {

  class ProcessController;

  class ProcessListener {

  public:
    ProcessListener(ProcessController* con)
      : m_con(con) {}
    ~ProcessListener() {}

  public:
    void run();

  private:
    ProcessController* m_con;

  };

}

#endif

