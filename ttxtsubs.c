/*
 * vdr-ttxtsubs - A plugin for the Linux Video Disk Recorder
 * Copyright (c) 2003 - 2008 Ragnar Sundblad <ragge@nada.kth.se>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include <vdr/plugin.h>
#include <vdr/status.h>
#include <vdr/vdrttxtsubshooks.h>
#include <vdr/menuitems.h>
#include <vdr/thread.h>
#include <vdr/config.h>
#include <semaphore.h>

#define TIMEMEASURE 0
#if TIMEMEASURE
#define TIMEVALSIMPL(tv, tv2) \
  if(tv2.tv_usec < tv.tv_usec) { \
    tv2.tv_usec += 1000000; \
    tv2.tv_sec -= 1; \
  }

#include <sys/time.h>
#include <time.h>
#endif

#include "ttxtsubsglobals.h"
#include "ttxtsubsdisplayer.h"
#include "ttxtsubsrecorder.h"
#include "utils.h"
#include "siinfo.h"
#include "ttxtsubspagemenu.h"
#include "ttxtsubschannelsettings.h"

#if defined(APIVERSNUM) && APIVERSNUM < 10600
#error "This version of ttxtsubs only works with vdr version >= 1.6.0!"
#endif

static const char *VERSION        = "0.0.8";
static const char *DESCRIPTION    = trNOOP("Teletext subtitles");

cTtxtsubsConf globals;
cTtxtSubsChannelSettings TtxtSubsChannelSettings;

// ISO 639-2 language codes in VDR order
// XXX should be replaced with something that allows for other languages and for real language names!
// <http://www.avio-systems.com/dtvcc/iso639-2.txt>
// <http://www.loc.gov/standards/iso639-2/englangn_ascii.html>
const char *gLanguages[][2] = { 
  {"",""},       //None
  {"eng",""},    //English
  {"deu","ger"}, //Deutsch
  {"slv",""},    //Slovenian
  {"ita",""},    //Italian
  {"dut","nld"}, //Dutch
  {"por",""},    //Portuguese
  {"fre","fra"}, //French
  {"nor",""},    //Norwegian
  {"fin","suo"}, //Finnish
  {"pol",""},    //Polish
  {"spa","esl"}, //Spanish
  {"gre","ell"}, //Greek
  {"swe","sve"}, //Swedish
  {"ron","rum"}, //Romanian
  {"hun",""},    //Hungarian
  {"cat",""},    //Catalanian
  {"rus",""},    //Russian
  {"hrv","scr"}, //Croatian
  {"est",""},    //Estonian
  {"dan",""},    //Danish
  {"cze","ces"}, //Czech
  {"tur",""}     //Turkish
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
  "ÅëëçíéêÜ",
  "Svenska",
  "Romaneste",
  "Magyar",
  "Català",
  "ÀãááÚØÙ",
  "Hrvatski",
  "Eesti",
  "Dansk",
  "Èesky",
  "Türkçe"
};
int gNumLanguages = sizeof(gLanguages) / sizeof(gLanguages[0]);

class cPluginTtxtsubs : public cPlugin, public cStatus, public cVDRTtxtsubsHookListener, public cThread {
public:
  cPluginTtxtsubs(void);
  virtual ~cPluginTtxtsubs();

  // -- cPlugin
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return tr(DESCRIPTION); }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Start(void);
  virtual void Stop(void);
  virtual void Housekeeping(void);
  virtual const char *MainMenuEntry(void);
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);

  // -- cStatus
 protected:
  virtual void ChannelSwitch(const cDevice *Device, int ChannelNumber);
  virtual void Replaying(const cControl *Control, const char *Name, const char *FileName, bool On);
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

  // -- cThread
  void Action(void);
  
  // -- internal
 private:
  void StartTtxtLive(const cDevice *Device, tChannelID chnid, int pid, int page);
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
  // wait for channel switch
  sem_t chswitchwait;
  cMutex getchmutex;
  int switchChannel;
  int lastc;
  const cDevice *switchDevice;
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
  int mTransparency;
  int mRed;
  int mGreen;
  int mBlue;
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
  sem_init(&chswitchwait,0,0);
  lastc=0;
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

  cThread::Start();

  TtxtSubsChannelSettings.Load(AddDirectory(ConfigDirectory("ttxtsubs"),"channelsettings.dat"));
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
      strncpy(globals.mLanguages[0][0], Setup.OSDLanguage, 4);
      globals.mLanguages[0][0][3] = '\0';
    }
  }

  //dprint("cPluginTtxtsubs::Start\n");

  HookAttach();

  return true;
}

void cPluginTtxtsubs::Stop(void)
{
  cThread::Cancel();
  StopTtxt();
}

void cPluginTtxtsubs::Housekeeping(void)
{
  // Perform any cleanup or other regular tasks.
}

const char *cPluginTtxtsubs::MainMenuEntry(void)
{
  switch(globals.mMainMenuEntry) {
  case 1:
    if(globals.mRealDoDisplay)
      return tr("Hide teletext subtitles");
    else
      return tr("Display teletext subtitles");
  case 2:
    if(globals.mBottomLB)
      return tr("Position Teletext Subtitles for 4:3/Anamorph");
    else
      return tr("Position Teletext Subtitles for Letterbox");
  case 3:
    return tr("Page Selection");
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
    globals.mRealDoDisplay = globals.mRealDoDisplay ? 0 : 1;
    return NULL;
  case 2:
    globals.mBottomLB = globals.mBottomLB ? 0 : 1;
    return NULL;
  case 3:
    return new cTtxtsubsPageMenu();
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
  if(!strcasecmp(Name, "Display")) { globals.mDoDisplay = atoi(Value); globals.mRealDoDisplay=globals.mDoDisplay; }
  else if(!strcasecmp(Name, "Record")) globals.mDoRecord = atoi(Value);
  else if(!strcasecmp(Name, "LiveDelay")) globals.mLiveDelay = atoi(Value);
  else if(!strcasecmp(Name, "ReplayDelay")) globals.mReplayDelay = atoi(Value);
  else if(!strcasecmp(Name, "MainMenuEntry")) globals.mMainMenuEntry = atoi(Value);
  else if(!strcasecmp(Name, "TextPos")) globals.mTextPos = atoi(Value);
  else if(!strcasecmp(Name, "BottomLB")) globals.mBottomLB = atoi(Value);
  else if(!strcasecmp(Name, "BottomAdj")) globals.mBottomAdj = atoi(Value);
  else if(!strcasecmp(Name, "FrenchSpecial")) globals.mFrenchSpecial = atoi(Value);
  else if(!strcasecmp(Name, "LineSpacing")) globals.mLineSpacing = atoi(Value);
  else if(!strcasecmp(Name, "DvbSources")) globals.mDvbSources = atoi(Value);
  else if(!strcasecmp(Name, "FontSize")) globals.mFontSize = atoi(Value);
  else if(!strcasecmp(Name, "FgColor")) globals.mFgColor = atoi(Value);
  else if(!strcasecmp(Name, "BgColor")) globals.mBgColor = atoi(Value);
  else if(!strcasecmp(Name, "CustomColor")) globals.mCustomColor = atoi(Value);
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

void cPluginTtxtsubs::Action(void)
{
  while(true) {
    sem_wait(&chswitchwait);
    int cn;
    const cDevice *dev;
    getchmutex.Lock();
    cn=switchChannel;
    dev=switchDevice;
    getchmutex.Unlock();
    if (cn!=lastc) {
      StopTtxt();
      lastc=0;
      cChannel *c = Channels.GetByNumber(cn);
      if(c) {
        //int manual_page = TtxtSubsChannelSettings.Page(c->GetChannelID());
        //if (manual_page>0) { 
        //  printf("ttxtsubs: manual page %03x selected\n", manual_page);
        //  if (c->Tpid()) StartTtxtLive(dev, c->GetChannelID(), c->Tpid(), manual_page);
        //} else if(manual_page==0) {
          struct ttxtinfo info;
          int pid, page;
          char lang[4] = "";
          bool geterror=true;
          if((globals.dvbSources() == 0) || ((globals.dvbSources() == 1) && c->IsSat()) || ((globals.dvbSources() == 2) && c->IsTerr()) || ((globals.dvbSources() == 3) && c->IsCable()))
          {
             int tries=0; //sometimes it doesn't find subtitles even if they're there
                          //the problem is that while the dish is moving it's picking the signal
                          //from a different satellite, so what really should be done is to
                          //check if the dish is positioned
             while(tries<=2) {
               while(geterror=GetTtxtInfo(dev->ActualDevice()->CardIndex(), c, &info)) {
                 esyslog("ttxtsubs: Error: GetTtxtInfo failed!");
                 cCondWait::SleepMs(1000);
                 getchmutex.Lock();
                 bool changed=(cn!=switchChannel);
                 getchmutex.Unlock();
                 if (changed) {
                   esyslog("ttxtsubs: Channel changed while looking for teletext info");
                   break;
                 }  
               }  
               if (!geterror) {
                 if(FindSubs(&info, &pid, &page, lang)) {
                   //dprint("CHANNELSWITCH, pid: %d page: %x\n", pid, page);
                   mPage = page; // XXX remember this for playback (temporary hack)!
                   for(int i = 0; i < gNumLanguages; i++) {
                      if(!memcmp(lang, gLanguages[i][0], 3) ||
                         !memcmp(lang, gLanguages[i][1], 3)) {
                        globals.mI18nLanguage = i - 1;
                      }
                   }
                   if(globals.mI18nLanguage < 0 || globals.mI18nLanguage >= I18nLanguages()->Size())
                     globals.mI18nLanguage = 0; // default to iso8859-1 if no predefined charset
                   StartTtxtLive(dev, c->GetChannelID(), pid, page);
                   FreeTtxtInfoData(&info);
                   break; 
                 } else {  //!FindSubs
                   FreeTtxtInfoData(&info);
                   tries++;
                 }  
               } else break; //if(!geterror)..
             } //while(tries<=2)  
          }
          lastc=cn;
        //}   
      }
    }  
  }
}

void cPluginTtxtsubs::ChannelSwitch(const cDevice *Device, int ChannelNumber)
{
#if TIMEMEASURE
  struct timeval tv, tv2;
  gettimeofday(&tv, NULL);  
#endif
  //dprint("cPluginTtxtsubs::ChannelSwitch(devicenr: %d, channelnr: %d) - mDispl: %x\n",  Device->DeviceNumber(), ChannelNumber, mDispl); // XXX

  if(Device->IsPrimaryDevice() && !Device->Replaying()) {
    if(ChannelNumber) {
      getchmutex.Lock();
      switchChannel=ChannelNumber;
      switchDevice=Device;
      getchmutex.Unlock();
      sem_post(&chswitchwait);
    } 
  }
#if TIMEMEASURE
  gettimeofday(&tv2, NULL);  
  TIMEVALSIMPL(tv, tv2);
  // tv2.tv_sec - tv.tv_sec + 1000000 * (tv2.tv_usec - tv.tv_usec);
  isyslog("ttxtsubs: Channel switch time: %d.%06d s",
	  tv2.tv_sec - tv.tv_sec, tv2.tv_usec - tv.tv_usec);
#endif
}

void cPluginTtxtsubs::Replaying(const cControl *Control, const char *Name, const char *FileName, bool On)
{
  //dprint("cPluginTtxtsubs::Replaying\n"); // XXX
  StopTtxt();
  if(On)
    StartTtxtPlay(mPage);
  // XXX this page number is just a fallback for old recordings which
  // don't have a index page
}

void cPluginTtxtsubs::PlayerTeletextData(uint8_t *p, int length)
{
  cTtxtSubsPlayer *r = dynamic_cast<cTtxtSubsPlayer *>(mDispl);

  if(!r) {
    esyslog("ttxtsubs: ERROR: not a cTtxtSubsPlayer!");
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

void cPluginTtxtsubs::StartTtxtLive(const cDevice *Device, tChannelID chnid, int pid, int page)
{
  //dprint("cPluginTtxtsubs::StartTtxtLive(devicenr: %d, pid: %d, page: %03x)\n",
  //	 Device->DeviceNumber(), pid, page); // XXX

  if(!mDispl) {
    cTtxtSubsLiveReceiver *r;
    //dprint("teletext subtitles started on pid %d\n", pid);
    mDispl = r = new cTtxtSubsLiveReceiver(chnid, pid, page); // takes 0.01-0.015 s
    if(!cDevice::PrimaryDevice()->ActualDevice()->AttachReceiver(r))
                                    // takes 0.02-0.04 s on a full featured card
      esyslog("ttxtsubs: Error: AttachReceiver failed!");
    ShowTtxt();
  } else
    esyslog("ttxtsubs: Error: StartTtxtLive called when already started!");
}

void cPluginTtxtsubs::StartTtxtPlay(int backup_page)
{
  //dprint("cPluginTtxtsubs::StartTtxtPlay\n");

  if(!mDispl) {
    isyslog("ttxtsubs: teletext subtitles replayer started with initial page %03x", backup_page);
    mDispl = new cTtxtSubsPlayer(backup_page);
    ShowTtxt();
  } else
    esyslog("ttxtsubs: Error: StartTtxtPlay called when already started!");
}

void cPluginTtxtsubs::StopTtxt(void)
{
  //dprint("cPluginTtxtsubs::StopTtxt\n");

  if(mDispl) {
    cTtxtSubsDisplayer *d = mDispl;
    HideTtxt();
    mDispl = NULL;
    delete d; // takes 0.03-0.04 s
  }
}

void cPluginTtxtsubs::ShowTtxt(void)
{
  if(globals.mRealDoDisplay)
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
  //dprint("cMenuSetupTtxtsubsLanguages::Store\n");
}

const char * mainMenuAlts[5] = {NULL, NULL, NULL, NULL, NULL};
const char * textPosAlts[4];
const char * dvbSources[5];
const char * textColors[12];
const char * const colorValues[17] = {"#00", "#11", "#22", "#33", "#44", "#55", "#66", "#77", "#88", "#99", "#AA", "#BB", "#CC", "#DD", "#EE", "#FF", NULL};

cMenuSetupTtxtsubs::cMenuSetupTtxtsubs(cPluginTtxtsubs *ttxtsubs, int doStore)
  :
  mTtxtsubs(ttxtsubs),
  mDoStore(doStore),
  mConf(globals)
{
  char *buf = NULL;
  //static char *mainMenuAlts[] = {"off", "Display on/off", "4:3/Letterbox", "This menu"};
  // can't get it to store changes in file
  if(mainMenuAlts[0] == NULL) {
    mainMenuAlts[0] = trVDR("off");
    mainMenuAlts[1] = tr("Display on/off");
    mainMenuAlts[2] = tr("4:3/Letterbox");
    mainMenuAlts[3] = tr("Page Mode");
    mainMenuAlts[4] = NULL;

    textPosAlts[0] = tr("Left");
    textPosAlts[1] = tr("Center");
    textPosAlts[2] = tr("Right");
    textPosAlts[3] = NULL;

    dvbSources[0] = tr("All");
    dvbSources[1] = tr("Only DVB-S");
    dvbSources[2] = tr("Only DVB-T");
    dvbSources[3] = tr("Only DVB-C");
    dvbSources[4] = NULL;

    textColors[0] = tr("Black");
    textColors[1] = tr("White");
    textColors[2] = tr("Red");
    textColors[3] = tr("Green");
    textColors[4] = tr("Yellow");
    textColors[5] = tr("Magenta");
    textColors[6] = tr("Blue");
    textColors[7] = tr("Cyan");
    textColors[8] = tr("Custom");
    textColors[9] = tr("Transparent");
    textColors[10] = tr("Grey");
    textColors[11] = NULL;
  }
  const int numTextPosAlts = sizeof(textPosAlts) / sizeof(textPosAlts[0]) - 1;
  const int numMainMenuAlts = sizeof(mainMenuAlts) / sizeof(mainMenuAlts[0]) - 1;
  const int numDvbSources = sizeof(dvbSources) / sizeof(dvbSources[0]) - 1;
  const int numTextColors = sizeof(textColors) / sizeof(textColors[0]) - 1;
  const int numColorValues = sizeof(colorValues) / sizeof(colorValues[0]) - 1;

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

  Add(new cMenuEditBoolItem(tr("Display Subtitles"), &mConf.mDoDisplay));
  Add(new cMenuEditBoolItem(tr("Record Subtitles"), &mConf.mDoRecord));
  Add(new cMenuEditIntItem(tr("Live Delay"), &mConf.mLiveDelay, 0, 5000));
  Add(new cMenuEditIntItem(tr("Replay Delay"), &mConf.mReplayDelay, 0, 5000));
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
  Add(new cMenuEditIntItem( tr("Line Spacing Adjust"),
			    &mConf.mLineSpacing, -25, 25));
  Add(new cMenuEditBoolItem(tr("Workaround for some French chns"),
			    &mConf.mFrenchSpecial));
  if(mConf.mDvbSources < 0 || mConf.mDvbSources >= numDvbSources)
    mConf.mDvbSources = 0;  // menu item segfaults if out of range
  Add(new cMenuEditStraItem(tr("DVB Source Selection"),
                          &mConf.mDvbSources, 4, dvbSources));
  Add(new cMenuEditIntItem(tr("Font Size (pixel)"), &mConf.mFontSize, 10, MAXFONTSIZE));
  if(mConf.mFgColor < 0 || mConf.mFgColor >= numTextColors)
    mConf.mFgColor = 1;  // menu item segfaults if out of range
  Add(new cMenuEditStraItem(tr("Text Color"), &mConf.mFgColor,
			    numTextColors, textColors));
  if(mConf.mBgColor < 0 || mConf.mBgColor >= numTextColors)
    mConf.mBgColor = 0;  // menu item segfaults if out of range
  Add(new cMenuEditStraItem(tr("Background Color"), &mConf.mBgColor,
			    numTextColors, textColors));

  // the color code is borrowed from AIO patches
  mBlue = (mConf.mCustomColor & 0x0000000F);
  if (mBlue < 0 || mBlue >= numColorValues) mBlue = 0;
  mGreen = (mConf.mCustomColor & 0x00000F00) >>  8;
  if (mGreen < 0 || mGreen >= numColorValues) mGreen = 0;
  mRed = (mConf.mCustomColor & 0x000F0000) >> 16;
  if (mRed < 0 || mRed >= numColorValues) mRed = 0;
  mTransparency = (mConf.mCustomColor & 0x0F000000) >> 24;
  if (mTransparency < 0 || mTransparency >= numColorValues) mTransparency = 0;
  asprintf(&buf, "%s: --------------------------------------------", tr("Custom Color"));
  cOsdItem *item = new cOsdItem(buf);
  item->SetSelectable(false);
  free(buf);
  Add(item);
  Add(new cMenuEditStraItem(tr("Red Value"),          &mRed,          numColorValues, colorValues));
  Add(new cMenuEditStraItem(tr("Green Value"),        &mGreen,        numColorValues, colorValues));
  Add(new cMenuEditStraItem(tr("Blue Value"),         &mBlue,         numColorValues, colorValues));
  Add(new cMenuEditStraItem(tr("Transparency Value"), &mTransparency, numColorValues, colorValues));

  for(int n = 0; n < MAXLANGUAGES; n++) {
    char str[100];
    const char *allowedc = "abcdefghijklmnopqrstuvwxyz";

    cOsdItem *item = new cOsdItem("--------------------------------------------------------");
    item->SetSelectable(false);
    Add(item);

    sprintf(str, "%s %d", tr("Language"), n + 1);
    if(mLanguageNo[n] >= 0) {
      Add(new cMenuEditStraItem(str, &mLanguageNo[n], gNumLanguages, gLanguageNames));
    } else {
      Add(new cMenuEditStrItem(str, mConf.mLanguages[n][0], 4, allowedc));
      Add(new cMenuEditStrItem(str, mConf.mLanguages[n][1], 4, allowedc));
    }

    sprintf(str, "%s %d %s", tr("Language"), n + 1, tr("Hearing Impaired"));
    Add(new cMenuEditBoolItem(str, &(mConf.mHearingImpaireds[n][0])));
  }

#ifdef TEST
  //AddSubMenu(new cMenuSetupTtxtsubsLanguages(mTtxtsubs));
#endif
}

cMenuSetupTtxtsubs::~cMenuSetupTtxtsubs(void)
{

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
  SetupStore("LiveDelay", mConf.mLiveDelay);
  SetupStore("ReplayDelay", mConf.mReplayDelay);
  SetupStore("TextPos", mConf.mTextPos);
  SetupStore("BottomLB", mConf.mBottomLB);
  SetupStore("BottomAdj", mConf.mBottomAdj);
  SetupStore("FrenchSpecial", mConf.mFrenchSpecial);
  SetupStore("MainMenuEntry", mConf.mMainMenuEntry);
  SetupStore("LineSpacing", mConf.mLineSpacing);
  SetupStore("DvbSources", mConf.mDvbSources);
  SetupStore("FontSize", mConf.mFontSize);
  SetupStore("FgColor", mConf.mFgColor);
  SetupStore("BgColor", mConf.mBgColor);
  mConf.mCustomColor = (mBlue              ) | (mBlue         <<  4) |
                       (mGreen        <<  8) | (mGreen        << 12) |
                       (mRed          << 16) | (mRed          << 20) |
                       (mTransparency << 24) | (mTransparency << 28);
  SetupStore("CustomColor", mConf.mCustomColor);

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
