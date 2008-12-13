
#include "teletext.h"

struct ttxt_data_field;
struct timeval;

class cOsd;
class cFont;

#define MAXTTXTROWS 5

class cTtxtSubsDisplay {
 public:
  cTtxtSubsDisplay(void);
  ~cTtxtSubsDisplay(void);

  void SetPage(int Pageno); // Pageno is 0x000 to 0x799
  void Hide(void);
  void Show(void);
  void TtxtData(const uint8_t *, uint64_t sched_time = 0);

 protected:
  void Clear(void);
  void ShowOSD();
  void ClearOSD(void);
  
 private:
  int mPageState;
  int mMag;
  int mNo;
  int mDoDisplay;
  struct ttxt_page page;
  cOsd *mOsd;
  cMutex mOsdLock;
  struct timeval *mLastDataTime;
  const cFont *mOsdFont;
};
