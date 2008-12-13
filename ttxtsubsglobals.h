
class cPluginTtxtsubs;
class cMenuSetupTtxtsubs;

#define MAXLANGUAGES 5

class cTtxtsubsConf {
  friend class cPluginTtxtsubs;
  friend class cMenuSetupTtxtsubs;

 public:
  cTtxtsubsConf(void)
    {
      mDoDisplay = 1;
      mDoRecord = 1;
      mMainMenuEntry = 0;
      mTextPos = 0;
      mBottomLB = 0;
      mBottomAdj = 0;
      mFrenchSpecial = 0;
      memset(mLanguages, 0, sizeof(mLanguages));
      memset(mHearingImpaireds, 0, sizeof(mHearingImpaireds));
    }

 public:
  int doDisplay(void) {return mDoDisplay;}
  int doRecord(void) {return mDoRecord;}
  int mainMenuEntry(void) {return mMainMenuEntry;}
  int textPos(void) {return mTextPos;}
  int bottomLB(void) {return mBottomLB;}
  int bottomAdj(void) {return mBottomAdj;}
  int frenchSpecial(void) {return mFrenchSpecial;}
  char (*languages(void))[MAXLANGUAGES][2][4] {return &mLanguages;}
  int (*hearingImpaireds(void))[MAXLANGUAGES][2] {return &mHearingImpaireds;}

  int langChoise(const char *lang, const int HI);

 protected:
  int mDoDisplay;
  int mDoRecord;
  int mMainMenuEntry;
  int mTextPos;
  int mBottomLB;
  int mBottomAdj;
  int mFrenchSpecial;
  char mLanguages[MAXLANGUAGES][2][4];
  int mHearingImpaireds[MAXLANGUAGES][2];
};

extern cTtxtsubsConf globals;
