/*
 * ttxtsubs.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: ttxtsubs.c,v 1.15 2003/06/20 02:35:37 ragge Exp $
 */

#include <vdr/plugin.h>
#include <vdr/status.h>
#include <vdr/vdrttxtsubshooks.h>
#include <vdr/menuitems.h>
#include <vdr/config.h>

#include "ttxtsubsdisplayer.h"
#include "ttxtsubsrecorder.h"
#include "utils.h"
#include "siinfo.h"
#include "ttxtsubs.h"

static const char *VERSION        = "0.0.3";
static const char *DESCRIPTION    = "Teletext subtitles";
//static const char *MAINMENUENTRY  = "Ttxtsubs";

// ISO 639-2 language codes in VDR order
// XXX should be replaced by something that allows for other languages and for real language names!
// <http://www.loc.gov/standards/iso639-2/englangn_ascii.html>
char *gLanguages[] = 
{ "eng", //English
    "deu", //Deutsch
    "slv", //Slovenian
    "ita", //Italian
    "dut", //"nld"? Dutch
    "por", //Portuguese
    "fre", //"fra"? French
    "nor", //Norwegian
    "fin", //Finnish
    "pol", //Polish
    "spa", //Spanish
    "gre", //Greek
    "swe", //"sve? Swedish
    "rom", //Romanian
    "hun", //Hungarian
    "cat", //Catalanian
  // Not in translations!
  "dan" //Danish
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
  //virtual const char *MainMenuEntry(void) { return MAINMENUENTRY; }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);

  // -- Setup stuff
  virtual char *Language(void) { return mLanguage; };
  virtual int HearingImpaired(void) { return mHearingImpaired; };
  virtual int Record(void) { return mRecord; };

  virtual void SetLanguage(char *lang) { strncpy(mLanguage, lang, 4); mLanguage[3] = '\0'; };
  virtual void SetHearingImpaired(int hi) { mHearingImpaired = hi; };
  virtual void SetRecord(int record) { mRecord = record; };

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

private:
  // Add any member variables or functions you may need here.
  cTtxtSubsDisplayer *mRec;

  // Setup items
  char mLanguage[4];
  int mHearingImpaired;
  int mRecord;

  // ugly hack for now
  int mPage;
};

class cMenuSetupTtxtsubs : public cMenuSetupPage {
 public:
  cMenuSetupTtxtsubs(cPluginTtxtsubs *ttxtsubs);
 protected:
  virtual void Store(void);
 private:
  cPluginTtxtsubs *mTtxtsubs;
  char mLanguage[4];
  int mLanguageNo;
  int mHearingImpaired;
  int mRecord;
};


cPluginTtxtsubs::cPluginTtxtsubs(void)
  :
  mRec(NULL),
  mHearingImpaired(0),
  mRecord(1),
  mPage(0x199)
{
  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!

  strncpy(mLanguage, "unk", 4);
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

  if(!memcmp(mLanguage, "unk", 3)) {
    int n = Setup.OSDLanguage;
    if(n > gNumLanguages) {
      strncpy(mLanguage, "eng", 4);
    } else {
      strncpy(mLanguage, gLanguages[n], 4);
      mLanguage[3] = '\0';
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

cOsdObject *cPluginTtxtsubs::MainMenuAction(void)
{
  // Perform the action when selected from the main VDR menu.
  return NULL;
}

cMenuSetupPage *cPluginTtxtsubs::SetupMenu(void)
{
  return new cMenuSetupTtxtsubs(this);
}

bool cPluginTtxtsubs::SetupParse(const char *Name, const char *Value)
{
  if(!strcasecmp(Name, "Language")) { strncpy(mLanguage, Value, 4); mLanguage[3] = '\0'; }
  else if(!strcasecmp(Name, "HearingImpaired")) mHearingImpaired = atoi(Value);
  else if(!strcasecmp(Name, "Record")) mRecord = atoi(Value);
  else
    return false;
  return true;
}

void cPluginTtxtsubs::ChannelSwitch(const cDevice *Device, int ChannelNumber)
{
  if(ChannelNumber) {
    cChannel *c = Channels.GetByNumber(ChannelNumber);
    if(c) {
      struct ttxtinfo info;
      int pid, page;
      
      if(GetTtxtInfo(Device->DeviceNumber(), c->Sid(), c->Vpid(), &info)) {
	fprintf(stderr, "ttxtsubs: Error: GetTtxtInfo failed!\n");
      } else {
	if(FindSubs(&info, mLanguage, mHearingImpaired, &pid, &page)) {
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

void cPluginTtxtsubs::Replaying(const cControl *Control, const char *Name)
{
  StopTtxt();
  StartTtxtPlay(mPage); // XXX should get page in some other way!
}

void cPluginTtxtsubs::PlayerTeletextData(uint8_t *p, int length)
{
  cTtxtSubsPlayer *r = dynamic_cast<cTtxtSubsPlayer *>(mRec);

  if(!r) {
    fprintf(stderr, "ttxtsubs: ERROR: not a cTtxtSubsPlayer!\n");
    return;
  }

  r->PES_data(p, length);
}

cTtxtSubsRecorderBase *cPluginTtxtsubs::NewTtxtSubsRecorder(cDevice *dev, const cChannel *ch)
{
  if(mRecord)
    return new cTtxtSubsRecorder(dev, ch, mLanguage, mHearingImpaired);
  else
    return NULL;
}


// --  internal

void cPluginTtxtsubs::StartTtxtLive(const cDevice *Device, int pid, int page)
{
  //dprint("cPluginTtxtsubs::StartTtxtLive\n");

#if 0
  return; // XXX TEST - No live subs
#endif

  if(!mRec) {
    cTtxtSubsLiveReceiver *r;
    //dprint("teletext subtitles started on pid %d\n", pid);
    mRec = r = new cTtxtSubsLiveReceiver(pid, page);
    if(!cDevice::PrimaryDevice()->AttachReceiver(r))
      fprintf(stderr, "ttxtsubs: Error: AttachReceiver failed!\n"); //
  } else
    fprintf(stderr, "ttxtsubs: Error: StartTtxtLive called when already started!\n");
}

void cPluginTtxtsubs::StartTtxtPlay(int backup_page)
{
  //dprint("cPluginTtxtsubs::StartTtxtPlay\n");

  if(!mRec) {
    dprint("ttxtsubs: teletext subtitles replayer started with initial page %03x\n", backup_page);
    mRec = new cTtxtSubsPlayer(mLanguage, mHearingImpaired, backup_page);
  } else
    fprintf(stderr, "ttxtsubs: Error: StartTtxtPlay called when already started!\n");
}

void cPluginTtxtsubs::StopTtxt(void)
{
  //dprint("cPluginTtxtsubs::StopTtxt\n");

  if(mRec) {
    HideTtxt();
    delete mRec;
    mRec = NULL;
  }
}

void cPluginTtxtsubs::ShowTtxt(void)
{
  if(mRec)
    mRec->ShowDisplay();
}

void cPluginTtxtsubs::HideTtxt(void)
{
  if(mRec)
    mRec->HideDisplay();
}

// ----- cMenuSetupTtxtsubs -----

cMenuSetupTtxtsubs::cMenuSetupTtxtsubs(cPluginTtxtsubs *ttxtsubs)
  :
  mTtxtsubs(ttxtsubs),
  mLanguageNo(-1),
  mHearingImpaired(mTtxtsubs->HearingImpaired()),
  mRecord(mTtxtsubs->Record())
{
  memcpy(mLanguage, ttxtsubs->Language(), 3);
  mLanguage[3] = '\0';

  for(int i = 0; i < gNumLanguages; i++) {
    if(!memcmp(mLanguage, gLanguages[i], 3)) {
      mLanguageNo = i;
      break;
    }
  }

  if(mLanguageNo >= 0)
    Add(new cMenuEditStraItem(tr("Language"), &mLanguageNo, gNumLanguages, gLanguages));
  else
    Add(new cMenuEditStrItem(tr("Language"), mLanguage, 4, "abcdefghijklmnopqrstuvwxyz"));
  
  Add(new cMenuEditBoolItem(tr("Hearing Impaired"), &mHearingImpaired, tr("no"), tr("yes")));
  Add(new cMenuEditBoolItem(tr("Record Subtitles"), &mRecord, tr("no"), tr("yes")));
}

void cMenuSetupTtxtsubs::Store(void)
{
  if(mLanguageNo >= 0) {
    SetupStore("Language", gLanguages[mLanguageNo]);
    mTtxtsubs->SetLanguage(gLanguages[mLanguageNo]);
  } else {
    SetupStore("Language", mLanguage);
    mTtxtsubs->SetLanguage(mLanguage);
  }
  
  SetupStore("HearingImpaired", mHearingImpaired);
  mTtxtsubs->SetHearingImpaired(mHearingImpaired);

  SetupStore("Record", mRecord);
  mTtxtsubs->SetRecord(mRecord);
}


VDRPLUGINCREATOR(cPluginTtxtsubs); // Don't touch this!
