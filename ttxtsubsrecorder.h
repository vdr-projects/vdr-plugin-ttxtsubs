
class cDevice;
class cChannel;
class cTtxtSubsReceiver;
struct ttxtinfo;

class cTtxtSubsRecorder : public cTtxtSubsRecorderBase {
 public:
  cTtxtSubsRecorder(cDevice *dev, const cChannel *ch);
  virtual ~cTtxtSubsRecorder();

  // returns a PES packet if there is data to add to the recording
  virtual uint8_t *GetPacket(uint8_t **buf, size_t *len);
  virtual void DeviceAttach(void);

 private:
  cDevice *mDev;
  int mSid;
  int mVid;
  ttxtinfo *mTtxtinfo;
  cTtxtSubsReceiver *mReceiver;
  uint8_t *mPacketBuffer;
};
