
class cRingBufferFrame;

class cTtxtSubsReceiver : public cReceiver
{
 public:
  cTtxtSubsReceiver(int Ca, struct ttxtpidinfo *PI);
  virtual ~cTtxtSubsReceiver();

  // returns pointer buf if there is new data
  uint8_t *Get(uint8_t *buf);
  // wait for more data
  void Wait(void);

  void IndexPage(char ***lines, int *linecount);

 protected:
  virtual void Activate(bool On);
  virtual void Receive(uchar *Data, int Length);

  virtual void AddIndexInfo(char *lang, int type, uint16_t page);
  virtual void EndIndexInfo(void);

 private:
  cMutex mGetMutex;
  cCondVar mGetCond;
  cRingBufferFrame mRingBuf;
  struct ttxtpidinfo mPI;
  cTtxtSubsFilter mFilter;
  uint8_t mIndexPage[24][46];
  uint8_t mIndexPageLines;
  uint8_t mIndexPageCol;
  uint16_t mIndexPageNo;
};
