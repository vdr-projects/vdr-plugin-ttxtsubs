
#include <vdr/receiver.h>
#include <vdr/ringbuffer.h>

class cTtxtSubsDisplay;

class cTtxtSubsDisplayer : public cThread {
 public:
  cTtxtSubsDisplayer(int textpage);
  ~cTtxtSubsDisplayer(void);
  void ShowDisplay(void);
  void HideDisplay(void);

 protected:
  virtual void Action(void);

 protected:
  cTtxtSubsDisplay *mDisp;
  cMutex mGetMutex;
  cCondVar mGetCond;
  cRingBufferFrame mRingBuf;
  int mRun;
};

class cTtxtSubsLiveReceiver : public cReceiver, public cTtxtSubsDisplayer {
 public:
  cTtxtSubsLiveReceiver(int Pid, int textpage);

 protected:
  virtual void Activate(bool On);
  virtual void Receive(uchar *Data, int Length);
};

class cTtxtSubsPlayer : public cTtxtSubsDisplayer {
 public:
  cTtxtSubsPlayer(char *lang, int HI, int backup_textpage);
  virtual void PES_data(uchar *Data, int Length);

 private:
  void SearchLanguagePage(uint8_t *p, int len);
  
  char mLanguage[4];
  int mHearingImpaired;
  int mHasFilteredStream;
  int mFoundLangPage;
  int mLangInfoState;
};
