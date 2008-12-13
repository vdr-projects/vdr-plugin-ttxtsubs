/*
 * ttxtsubs.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: ttxtsubs.c,v 1.22 2004/03/01 04:36:32 ragge Exp $
 */

#include <vdr/plugin.h>
#include <vdr/status.h>
#include <vdr/vdrttxtsubshooks.h>
#include <vdr/menuitems.h>
#include <vdr/config.h>

#include "ttxtsubsglobals.h"
#include "ttxtsubsdisplayer.h"
#include "ttxtsubsrecorder.h"
#include "utils.h"
#include "siinfo.h"
#include "ttxtsubs.h"

static const char *VERSION        = "0.0.5pre1";
static const char *DESCRIPTION    = "Teletext subtitles";

cTtxtsubsConf globals;

// ISO 639-2 language codes in VDR order
// XXX should be replaced with something that allows for other languages and for real language names!
// <http://www.avio-systems.com/dtvcc/iso639-2.txt>
// <http://www.loc.gov/standards/iso639-2/englangn_ascii.html>
char *gLanguages[][2] = { 
  "","",       //None
  "eng","",    //English
  "deu","ger", //Deutsch
  "slv","",    //Slovenian
  "ita","",    //Italian
  "dut","nld", //Dutch
  "por","",    //Portuguese
  "fre","fra", //French
  "nor","",    //Norwegian
  "fin","",    //Finnish
  "pol","",    //Polish
  "spa","esl", //Spanish
  "gre","ell", //Greek
  "swe","sve", //Swedish
  "ron","rum", //Romanian
  "hun","",    //Hungarian
  "cat","",    //Catalanian
  // Not in translations!
  "dan","" //Danish
};
const char *gLanguageNames[] = {
  "-",
  "English",
  "Deutsch",
  "Slovenski",
  "Italiano",
  "Nederlands",
  "Português",
  "Français",
  "Norsk",
  "suomi", // this is not a typo - it's really lowercase!
  "Polski",
  "Español",
  "Ellinika",
  "Svenska",
  "Romaneste",
  "Magyar",
  "Català",
  "Dansk"
};
int gNumLanguages = sizeof(gLanguages) / sizeof(gLanguages[0]);

class cPluginTtxtsubs : public cPlugin, public cStatus, public cVDRTtxtsubsHookListener {
public:
  cPluginTtxtsubs(void);
  virtual ~cPluginTtxtsubs();

  // -- cPlugin
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return DESCRIPTION; }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Start(void);
  virtual void Housekeeping(void);
  virtual const char *MainMenuEntry(void);
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);

  // -- cStatus
 protected:
  virtual void ChannelSwitch(const cDevice *Device, int ChannelNumber);
  virtual void Replaying(const cControl *Control, const char *Name);
               // The given player control has started replaying Name. Name is the name of the
               // recording, without any directory path. In case of a player that can't provide
               // a name, Name can be a string that identifies the player type (like, e.g., "DVD").
               // If Name is NULL, the replay has ended. 
  //  virtual void OsdClear(void) { ShowTtxt(); }
  //  virtual void OsdTitle(const char *Title) { HideTtxt(); }
  //  virtual void OsdStatusMessage(const char *Message) { HideTtxt(); }
  //  virtual void OsdHelpKeys(const char *Red, const char *Green, const char *Yellow, const char *Blue) { HideTtxt(); }
  //  virtual void OsdCurrentItem(const char *Text) { HideTtxt(); }
  //  virtual void OsdTextItem(const char *Text, bool Scroll) { HideTtxt(); }
  //  virtual void OsdChannel(const char *Text) { HideTtxt(); }
  //  virtual void OsdProgramme(time_t PresentTime, const char *PresentTitle, const char *PresentSubtitle, time_t FollowingTime, const char *FollowingTitle, const char *FollowingSubtitle) { HideTtxt(); }

  // -- cVDRTtxtsubsHookListener
  virtual void HideOSD(void) { HideTtxt(); }
  virtual void ShowOSD(void) { ShowTtxt(); }
  virtual void PlayerTeletextData(uint8_t *p, int length);
  virtual cTtxtSubsRecorderBase *NewTtxtSubsRecorder(cDevice *dev, const cChannel *ch);

  // -- internal
 private:
  void StartTtxtLive(const cDevice *Device, int pid, int page);
  void StartTtxtPlay(int page);
  void StopTtxt(void);
  void ShowTtxt(void);
  void HideTtxt(void);
  void parseLanguages(const char *val);
  void parseHIs(const char *val);

private:
  cTtxtSubsDisplayer *mDispl;

  char mOldLanguage[4]; // language chosen from previous version
  int mOldHearingImpaired; // HI setting chosen from previous version

  // ugly hack for now
  int mPage;
};

class cMenuSetupTtxtsubs : public cMenuSetupPage {
 public:
  cMenuSetupTtxtsubs(cPluginTtxtsubs *ttxtsubs, int doStore = 0);
  ~cMenuSetupTtxtsubs(void);
 protected:
  virtual void Store(void);
 private:
  cPluginTtxtsubs *mTtxtsubs;
  int mLanguageNo[MAXLANGUAGES];
  int mLangHI[MAXLANGUAGES];
  int mSavedFrenchSpecial;
  int mDoStore;
  cTtxtsubsConf mConf;
};


cPluginTtxtsubs::cPluginTtxtsubs(void)
  :
  mDispl(NULL),
  mOldHearingImpaired(0)
{
  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
  
  memset(mOldLanguage, 0, 4);
  strncpy(globals.mLanguages[0][0], "unk", 4);
}

cPluginTtxtsubs::~cPluginTtxtsubs()
{
  // Clean up after yourself!
}

const char *cPluginTtxtsubs::CommandLineHelp(void)
{
  // Return a string that describes all known command line options.
  return NULL;
}

bool cPluginTtxtsubs::ProcessArgs(int argc, char *argv[])
{
  // Implement command line argument processing here if applicable.
  return true;
}

bool cPluginTtxtsubs::Start(void)
{
  // Start any background activities the plugin shall perform.

  if(!memcmp(globals.mLanguages[0][0], "unk", 3)) {
    // no language found in setup
    if(strlen(mOldLanguage)) {
      // use old setup lang, first try to find it amongst known languages
      for(int i = 0; i < gNumLanguages; i++) {
	if(!memcmp(mOldLanguage, gLanguages[i][0], 3) ||
	   !memcmp(mOldLanguage, gLanguages[i][1], 3)) {
	  strncpy(globals.mLanguages[0][0], gLanguages[i][0], 4);
	  globals.mLanguages[0][0][3] = '\0';
	  strncpy(globals.mLanguages[0][1], gLanguages[i][1], 4);
	  globals.mLanguages[0][1][3] = '\0';
	}
      }
      if(!memcmp(globals.mLanguages[0][0], "unk", 3)) {
	// not found there, just copy it
	memcpy(globals.mLanguages[0][0], mOldLanguage, 3);
	globals.mLanguages[0][0][3] = '\0';
      }
      globals.mHearingImpaireds[0][0] = mOldHearingImpaired;
      globals.mHearingImpaireds[0][1] = mOldHearingImpaired;
    } else {
      // get lang from OSD lang
      int n = Setup.OSDLanguage + 1;
      if(n > gNumLanguages) {
	strncpy(globals.mLanguages[0][0], "eng", 4);
      } else {
	strncpy(globals.mLanguages[0][0], gLanguages[n][0], 4);
	globals.mLanguages[0][0][3] = '\0';
	strncpy(globals.mLanguages[0][1], gLanguages[n][1], 4);
	globals.mLanguages[0][1][3] = '\0';
      }
    }
  }

  //dprint("cPluginTtxtsubs::Start\n");

  HookAttach();

  return true;
}

void cPluginTtxtsubs::Housekeeping(void)
{
  // Perform any cleanup or other regular tasks.
}

const char *cPluginTtxtsubs::MainMenuEntry(void)
{
  switch(globals.mMainMenuEntry) {
  case 1:
    if(globals.mDoDisplay)
      return tr("Hide teletext subtitles");
    else
      return tr("Display teletext subtitles");
  case 2:
    if(globals.mBottomLB)
      return tr("Position Teletext Subtitles for 4:3/Anamorph");
    else
      return tr("Position Teletext Subtitles for Letterbox");
  case 3:
    return tr("Teletext Subtitles");
  default:
    return NULL;
  }

  return NULL;
}

cOsdObject *cPluginTtxtsubs::MainMenuAction(void)
{
  // Perform the action when selected from the main VDR menu.

  switch(globals.mMainMenuEntry) {
  case 1:
    globals.mDoDisplay = globals.mDoDisplay ? 0 : 1;
    return NULL;
  case 2:
    globals.mBottomLB = globals.mBottomLB ? 0 : 1;
    return NULL;
  case 3:
    return new cMenuSetupTtxtsubs(this, 1);
  default:
    return NULL;
  }

  return NULL;
}

cMenuSetupPage *cPluginTtxtsubs::SetupMenu(void)
{
  return new cMenuSetupTtxtsubs(this);
}

bool cPluginTtxtsubs::SetupParse(const char *Name, const char *Value)
{
  if(!strcasecmp(Name, "Display")) globals.mDoDisplay = atoi(Value);
  else if(!strcasecmp(Name, "Record")) globals.mDoRecord = atoi(Value);
  else if(!strcasecmp(Name, "MainMenuEntry")) globals.mMainMenuEntry = atoi(Value);
  else if(!strcasecmp(Name, "TextPos")) globals.mTextPos = atoi(Value);
  else if(!strcasecmp(Name, "BottomLB")) globals.mBottomLB = atoi(Value);
  else if(!strcasecmp(Name, "BottomAdj")) globals.mBottomAdj = atoi(Value);
  else if(!strcasecmp(Name, "FrenchSpecial")) globals.mFrenchSpecial = atoi(Value);
  else if(!strcasecmp(Name, "Languages")) parseLanguages(Value);
  else if(!strcasecmp(Name, "HearingImpaireds")) parseHIs(Value);
  // Handle old settings
  else if(!strcasecmp(Name, "Language")) {
    strncpy(mOldLanguage, Value, 4); mOldLanguage[3] = '\0'; }
  else if(!strcasecmp(Name, "HearingImpaired")) mOldHearingImpaired = atoi(Value);
  else
    return false;
  return true;
}

void cPluginTtxtsubs::ChannelSwitch(const cDevice *Device, int ChannelNumber)
{
  dprint("cPluginTtxtsubs::ChannelSwitch(devicenr: %d, channelnr: %d) - mDispl: %x\n",
	 Device->DeviceNumber(), ChannelNumber, mDispl); // XXX

  if(Device->IsPrimaryDevice()) {
    if(ChannelNumber) {
      cChannel *c = Channels.GetByNumber(ChannelNumber);
      if(c) {
	struct ttxtinfo info;
	int pid, page;
	
	if(GetTtxtInfo(Device->ActualDevice()->CardIndex(), ChannelNumber, c->Sid(), c->Vpid(), &info)) {
	  fprintf(stderr, "ttxtsubs: Error: GetTtxtInfo failed!\n");
	} else {
	  if(FindSubs(&info, &pid, &page)) {
	    //fprintf(stderr, "CHANNELSWITCH, pid: %d page: %x\n", pid, page);
	    mPage = page; // XXX remember this for playback (temporary hack)!
	    StartTtxtLive(Device, pid, page);
	  }
	  FreeTtxtInfoData(&info);
	}
      }
    } else
      StopTtxt();
  }
}

void cPluginTtxtsubs::Replaying(const cControl *Control, const char *Name)
{
  dprint("cPluginTtxtsubs::Replaying\n"); // XXX

  StopTtxt();
  StartTtxtPlay(mPage);
  // XXX this page number is just a fallback for old recordings which
  // don't have a index page
}

void cPluginTtxtsubs::PlayerTeletextData(uint8_t *p, int length)
{
  cTtxtSubsPlayer *r = dynamic_cast<cTtxtSubsPlayer *>(mDispl);

  if(!r) {
    fprintf(stderr, "ttxtsubs: ERROR: not a cTtxtSubsPlayer!\n");
    return;
  }

  r->PES_data(p, length);
}

cTtxtSubsRecorderBase *cPluginTtxtsubs::NewTtxtSubsRecorder(cDevice *dev, const cChannel *ch)
{
  if(globals.mDoRecord)
    return new cTtxtSubsRecorder(dev, ch);
  else
    return NULL;
}


// --  internal

void cPluginTtxtsubs::StartTtxtLive(const cDevice *Device, int pid, int page)
{
  dprint("cPluginTtxtsubs::StartTtxtLive(devicenr: %d, pid: %d, page: %03x)\n",
	 Device->DeviceNumber(), pid, page); // XXX

  if(!mDispl) {
    cTtxtSubsLiveReceiver *r;
    //dprint("teletext subtitles started on pid %d\n", pid);
    mDispl = r = new cTtxtSubsLiveReceiver(pid, page);
    if(!cDevice::PrimaryDevice()->ActualDevice()->AttachReceiver(r))
      fprintf(stderr, "ttxtsubs: Error: AttachReceiver failed!\n"); //
    ShowTtxt();
  } else
    fprintf(stderr, "ttxtsubs: Error: StartTtxtLive called when already started!\n");
}

void cPluginTtxtsubs::StartTtxtPlay(int backup_page)
{
  dprint("cPluginTtxtsubs::StartTtxtPlay\n");

  if(!mDispl) {
    dprint("ttxtsubs: teletext subtitles replayer started with initial page %03x\n", backup_page);
    mDispl = new cTtxtSubsPlayer(backup_page);
    ShowTtxt();
  } else
    fprintf(stderr, "ttxtsubs: Error: StartTtxtPlay called when already started!\n");
}

void cPluginTtxtsubs::StopTtxt(void)
{
  dprint("cPluginTtxtsubs::StopTtxt\n");

  if(mDispl) {
    cTtxtSubsDisplayer *d = mDispl;
    HideTtxt();
    mDispl = NULL;
    delete d;
  }
}

void cPluginTtxtsubs::ShowTtxt(void)
{
  if(globals.mDoDisplay)
    if(mDispl)
      mDispl->ShowDisplay();
}

void cPluginTtxtsubs::HideTtxt(void)
{
  if(mDispl)
    mDispl->HideDisplay();
}

void cPluginTtxtsubs::parseLanguages(const char *val) {
  size_t i;
  const char *p = val;
  const char *p2;
  size_t len;

  for(i = 0; i < (MAXLANGUAGES*2); i++) {
    if(*p == '\0')
      break; // end of string

    p2 = strchr(p, ','); // find end of entry
    if(p2) {
      len = p2 - p;
    } else { // no more , found
      len = strlen(p);
    }

    if(len) {
      size_t trlen = len;
      if(trlen > 3)
	trlen = 3;
      memcpy(globals.mLanguages[i/2][i%2], p, trlen);
      globals.mLanguages[i/2][i%2][trlen] = '\0';
    }

    p += len + 1;
  }
}

void cPluginTtxtsubs::parseHIs(const char *val)
{
  size_t i;

  for(i = 0; i < (MAXLANGUAGES*2); i++) {
    if(val[i] == '\0')
      break;
    globals.mHearingImpaireds[i/2][i%2] = val[i] != '0';
  }
}

// ----- cMenuSetupTtxtsubs -----

//#define TEST
class cMenuSetupTtxtsubsLanguages : public cMenuSetupPage {
 public:
  cMenuSetupTtxtsubsLanguages(cPluginTtxtsubs *ttxtsubs);
 protected:
  virtual void Store(void);
 private:
  cPluginTtxtsubs *mTtxtsubs;
};

cMenuSetupTtxtsubsLanguages::cMenuSetupTtxtsubsLanguages(cPluginTtxtsubs *ttxtsubs)
  :
  mTtxtsubs(ttxtsubs)
{}

void cMenuSetupTtxtsubsLanguages::Store(void)
{
  fprintf(stderr, "cMenuSetupTtxtsubsLanguages::Store\n");
}

cMenuSetupTtxtsubs::cMenuSetupTtxtsubs(cPluginTtxtsubs *ttxtsubs, int doStore)
  :
  mTtxtsubs(ttxtsubs),
  mDoStore(doStore),
  mConf(globals)
{
  //static char *mainMenuAlts[] = {"off", "Display on/off", "4:3/Letterbox", "This menu"};
  // can't get it to store changes in file
  static char *mainMenuAlts[] = {"off", "Display on/off", "4:3/Letterbox"};
  static int numMainMenuAlts = sizeof(mainMenuAlts) / sizeof(mainMenuAlts[0]);
  static char *textPosAlts[] = {"Left", "Center", "Right"};
  static int numTextPosAlts = sizeof(textPosAlts) / sizeof(textPosAlts[0]);

  mSavedFrenchSpecial = mConf.mFrenchSpecial;

  for(int n = 0; n < MAXLANGUAGES; n++) {
    mLanguageNo[n] = -1;
    mLangHI[n] = mConf.mHearingImpaireds[n][0];

    for(int i = 0; i < gNumLanguages; i++) {
      if(!strncmp(mConf.mLanguages[n][0], gLanguages[i][0], 4) &&
	 !strncmp(mConf.mLanguages[n][1], gLanguages[i][1], 4)) {
	mLanguageNo[n] = i;
	break;
      }
    }
  }

  Add(new cMenuEditBoolItem(tr("Display Subtitles"), &mConf.mDoDisplay, tr("no"), tr("yes")));
  Add(new cMenuEditBoolItem(tr("Record Subtitles"), &mConf.mDoRecord, tr("no"), tr("yes")));
  if(mConf.mMainMenuEntry < 0 || mConf.mMainMenuEntry >= numMainMenuAlts)
    mConf.mMainMenuEntry = 0;  // menu item segfaults if out of range
  Add(new cMenuEditStraItem(tr("Main Menu Alternative"), &mConf.mMainMenuEntry,
			    numMainMenuAlts, mainMenuAlts));
  if(mConf.mTextPos < 0 || mConf.mTextPos >= numTextPosAlts)
    mConf.mTextPos = 0;  // menu item segfaults if out of range
  Add(new cMenuEditStraItem(tr("Text Horizontal Position"), &mConf.mTextPos,
			    numTextPosAlts, textPosAlts));
  Add(new cMenuEditBoolItem(tr("Text Vertical Position"),
			    &mConf.mBottomLB, tr("4:3/Anamorph"), tr("Letterbox")));
  Add(new cMenuEditIntItem(tr("Text Vertical Adjust"),
			    &mConf.mBottomAdj, -100, 45));
  Add(new cMenuEditBoolItem(tr("Workaround for some French chns"),
			    &mConf.mFrenchSpecial, tr("no"), tr("yes")));

  for(int n = 0; n < MAXLANGUAGES; n++) {
    char str[100];
    char *allowedc = "abcdefghijklmnopqrstuvwxyz";

    cOsdItem *item = new cOsdItem("----------------------------");
    item->SetColor(clrCyan);
    Add(item);

    sprintf(str, "Language %d", n + 1);
    if(mLanguageNo[n] >= 0) {
      Add(new cMenuEditStraItem(tr(str), &mLanguageNo[n], gNumLanguages, gLanguageNames));
    } else {
      Add(new cMenuEditStrItem(tr(str), mConf.mLanguages[n][0], 4, allowedc));
      Add(new cMenuEditStrItem(tr(str), mConf.mLanguages[n][1], 4, allowedc));
    }

    sprintf(str, "Language %d Hearing Impaired", n + 1);
    Add(new cMenuEditBoolItem(tr(str), &(mConf.mHearingImpaireds[n][0]),
			      tr("no"), tr("yes")));
  }

#ifdef TEST
  //AddSubMenu(new cMenuSetupTtxtsubsLanguages(mTtxtsubs));
#endif
}

cMenuSetupTtxtsubs::~cMenuSetupTtxtsubs(void)
{
  if(mSavedFrenchSpecial != mConf.mFrenchSpecial)
    ClearSICache();

  if(mDoStore) {
    Store();
    //    Setup.Save(); // Can't get it to write to conf file, menu item disabled.
  }
}

void cMenuSetupTtxtsubs::Store(void)
{
  for(int n=0; n < MAXLANGUAGES; n++) {
    if(mLanguageNo[n] >= 0) {
      strncpy(mConf.mLanguages[n][0], gLanguages[mLanguageNo[n]][0], 4);
      mConf.mLanguages[n][0][3] = '\0';
      strncpy(mConf.mLanguages[n][1], gLanguages[mLanguageNo[n]][1], 4);
      mConf.mLanguages[n][1][3] = '\0';
    }

    mConf.mHearingImpaireds[n][1] = mConf.mHearingImpaireds[n][0];
  }

  SetupStore("Display", mConf.mDoDisplay);
  SetupStore("Record", mConf.mDoRecord);
  SetupStore("TextPos", mConf.mTextPos);
  SetupStore("BottomLB", mConf.mBottomLB);
  SetupStore("BottomAdj", mConf.mBottomAdj);
  SetupStore("FrenchSpecial", mConf.mFrenchSpecial);
  SetupStore("MainMenuEntry", mConf.mMainMenuEntry);

  char lstr[MAXLANGUAGES*2*4 + 1];
  char histr[MAXLANGUAGES*2 + 1];
  lstr[0] = '\0';
  histr[0] = '\0';
  for(int n=0; n < MAXLANGUAGES; n++) {
    strncat(lstr, mConf.mLanguages[n][0], 3);
    strcat(lstr, ",");
    strncat(lstr, mConf.mLanguages[n][1], 3);
    if(n != (MAXLANGUAGES - 1))
      strcat(lstr, ",");
      
    strcat(histr, mConf.mHearingImpaireds[n][0] ? "1" : "0");
    strcat(histr, mConf.mHearingImpaireds[n][1] ? "1" : "0");
  }
  SetupStore("Languages", lstr);
  SetupStore("HearingImpaireds", histr);

  globals = mConf;
}


// returns the index to choise number for the given language,
// lower is more preferred. The result is times two, plus one
// if the HI wanted but not found, or -1 if no match.
// Non HI wanted but found is no match.

int cTtxtsubsConf::langChoise(const char *lang, const int HI)
{
  size_t i, j;
  int result = -1;
  
  for(i = 0; i < MAXLANGUAGES; i++) {
    for(j = 0; j < 2; j++) {
      if(!mLanguages[i][j][0])
	continue;

      if(!memcmp(lang, mLanguages[i][j], 3)) {
	if( ( HI && mHearingImpaireds[i][j] ) || 
	    ( !HI && !mHearingImpaireds[i][j] ) ) {
	  result = i*2;
	  goto x;
	}
	if( !HI && mHearingImpaireds[i][j] ) {
	  result = 1 + i*2;
	  goto x;
	}
      }
    }
  }

 x:
  return result;
}


VDRPLUGINCREATOR(cPluginTtxtsubs); // Don't touch this!
