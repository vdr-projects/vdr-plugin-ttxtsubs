
#include "ttxtsubsdisplayer.h"
#include "ttxtsubsdisplay.h"
#include "utils.h"
#include "ttxtsubs.h"
#include "siinfo.h"
#include "ttxtsubsglobals.h"

// ----- class cTtxtSubsDisplayer -----

cTtxtSubsDisplayer::cTtxtSubsDisplayer(int textpage)
  :
  mDisp(NULL),
  mGetMutex(),
  mGetCond(),
  mRingBuf(18800, true),
  mRun(0)
{
  mDisp = new cTtxtSubsDisplay();
  mDisp->SetPage(textpage);

  mRun = 1;
  this->Start(); // start thread
}

cTtxtSubsDisplayer::~cTtxtSubsDisplayer(void)
{
  // stop thread
  mRun = false;
  mGetCond.Broadcast();
  this->Cancel(5);

  if(mDisp) {
    cTtxtSubsDisplay *tmp = mDisp;
    mDisp = NULL;
    delete tmp;
  }
}

// handled messages in ring buffer - runs in a thread of its own
void cTtxtSubsDisplayer::Action(void)
{
  while(mRun) {
    cFrame *f;

    f = mRingBuf.Get();

    if(f) {
      uint64_t sched_time;
      memcpy(&sched_time, f->Data() + 46, sizeof(sched_time));
      mDisp->TtxtData(f->Data(), sched_time);
      mRingBuf.Drop(f);
    } else {
      // wait for more data
      mGetMutex.Lock();
      mGetCond.TimedWait(mGetMutex, 500);
      mDisp->TtxtData(NULL); // give display some idle time
      mGetMutex.Unlock();
    }
  }
}

void cTtxtSubsDisplayer::ShowDisplay(void)
{
  mDisp->Show();
}

void cTtxtSubsDisplayer::HideDisplay(void)
{
  mDisp->Hide();
}


// ----- class cTtxtSubsLiveReceiver -----

cTtxtSubsLiveReceiver::cTtxtSubsLiveReceiver(tChannelID ChnId, int Pid, int textpage)
  :
  cReceiver(ChnId, -1, Pid),
  cTtxtSubsDisplayer(textpage)
{
}

cTtxtSubsLiveReceiver::~cTtxtSubsLiveReceiver(void)
{
  Detach();
}

void cTtxtSubsLiveReceiver::Activate(bool On)
{
  //dprint("cTtxtSubsLiveReceiver::Activate: On:%d\n", On);
}

// Take TS packets and break out the teletext data
// Buffer the data for processing in a separate thread
// XXX We should do some filtering here to avoid unneccessary load!
void cTtxtSubsLiveReceiver::Receive(uchar *Data, int Length)
{
  int i;

  if(Length != 188) // should never happen
    return;

  if(Data[1] & 0x80) // transport_error_indicator
    return;

  // payload_unit_start_indicator
  for(i = (Data[1] & 0x40) ? 1 : 0; i < 4; i++) {
    if(0xff == Data[4 + i*46]) // stuffing data
      continue;

    uint64_t sched_time = cTimeMs::Now() + globals.liveDelay(); 
    cFrame *f = new cFrame(Data + 4 + i*46, 46 + sizeof(sched_time));
    memcpy(f->Data() + 46, &sched_time, sizeof(sched_time));
    mRingBuf.Put(f);
    mGetCond.Broadcast();
  }
}

// ----- class cTtxtSubsPlayer -----

cTtxtSubsPlayer::cTtxtSubsPlayer(int backup_textpage)
  :
  cTtxtSubsDisplayer(backup_textpage),
  mHasFilteredStream(0),
  mFoundLangPage(0),
  mLangChoise(1000),
  mLangInfoState(0)
{
}

// Take PES packets and break out the teletext data
// Buffer the data for processing in a separate thread
// XXX We should do some filtering here to avoid unneccessary load!
void cTtxtSubsPlayer::PES_data(uchar *p, int Length)
{
  int i;

  //printf("cTtxtSubsPlayer: len: %d\n", Length); // XXX

  if(Length < 46 || p[0] != 0 || p[1] != 0 || p[2] != 1 || p[3] != 0xbd || p[8] != 0x24 ||
     p[45] < 0x10 || p[45] >= 0x20) {
    fprintf(stderr, "cTtxtSubsPlayer::PES_data: bad indata!\n");
  }

  // Recorded teletext typically has payload type 0x10.
  // We use the payload type 0x1f to indicate that we have our own
  // filtered stream, if we find that we only use that.

  if(p[45] == 0x1f && !mHasFilteredStream)
    mHasFilteredStream = 1;

  if(mHasFilteredStream && p[45] != 0x1f)
    return; // other teletext data, ignore

  if(mHasFilteredStream && !mFoundLangPage)
    SearchLanguagePage(p, Length);

  // payload_unit_start_indicator
  for(i = 1; (i*46) < Length ; i++) {
    if(0xff == p[i*46]) // stuffing data
      continue;

    uint64_t sched_time=cTimeMs::Now() + globals.replayDelay(); 
    cFrame *f = new cFrame(p + i*46, 46 + sizeof(sched_time));
    memcpy(f->Data() + 46, &sched_time, sizeof(sched_time));
    mRingBuf.Put(f);
    mGetCond.Broadcast();
  }
}

static void copy_inv_strip_par(uint8_t *dest, uint8_t *src, size_t len)
{
  size_t i;
  for(i = 0; i < len; i++)
    dest[i] = invtab[src[i]] & 0x7f;
}

void cTtxtSubsPlayer::SearchLanguagePage(uint8_t *p, int len)
{
  const char *infoline = "Subtitles Index Page";
  int foundlines = 0;

  if(len < (3*46))
     return;

  struct ttxt_data_field *d;
  for(d = ((struct ttxt_data_field *) p) + 1; ((uint8_t *) d) < (p + len + 45); d++) {
    int mp = UNHAM_INV(d->mag_addr_ham[0], d->mag_addr_ham[1]);
    //int mag = mp & 0x7;
    int packet = (mp >> 3) & 0x1f;
    uint8_t buf[40];

    if(d->data_unit_id != 0x02)
      break; // a subtitle line

    switch(foundlines) {
    case 0:
      if(packet != 0) // need a Y0 first
	return;
      foundlines++;
      break;
    case 1:
      if(packet != 1) // need a Y1
	return;
      copy_inv_strip_par(buf, d->data, sizeof(buf));
      if(memcmp((char *) buf, infoline, strlen(infoline)))
	return;
      foundlines++;
      break;
    case 2:
      mLangInfoState++;
      if(packet < 2) // need a Y2 or more
	return;
      if(mLangInfoState == 3)
	fprintf(stderr, "ttxtsubs: Chosen Language not found in recording, available languages:\n");
      copy_inv_strip_par(buf, d->data, sizeof(buf));
      for(size_t i = 0; i < 40; i += 8) {
	if(mLangInfoState == 3 && buf[i] >= 'a' && buf[i] <= 'z')
	  fprintf(stderr, "          %c%c%c: %c%c%c %s\n", buf[i+4], buf[i+5], buf[i+6],
		  buf[i], buf[i+1], buf[i+2], buf[i+3] == ' ' ? "" :
		  buf[i+3] == 'h' ? "(Hearing Impaired)" : "(Unknown type)");

	if(buf[i] >= 'a' && buf[i] <= 'z' &&
	   buf[i+1] >= 'a' && buf[i+1] <= 'z' &&
	   buf[i+2] >= 'a' && buf[i+2] <= 'z' &&
	   ((buf[i+3] == ' ') || (buf[i+3] == 'h')) &&
	   buf[i+4] >= '1' && buf[i+4] <= '8' && 
	   buf[i+5] >= '0' && buf[i+5] <= '9' && 
	   buf[i+6] >= '0' && buf[i+6] <= '9' && 
	   buf[i+7] == ' ') {
	      int ch = globals.langChoise((char *)buf+i, buf[i+3] == 'h');
	      unsigned int page =
		((buf[i+4] - '0') << 8) +
		((buf[i+5] - '0') << 4) +
		(buf[i+6] - '0');
	      if(page >= 0x100 && page < 0x900) {
		if(page >= 0x800)
		  page -= 0x800;

		if(ch >= 0 && ch < mLangChoise) {
		  mLangChoise = ch;
		  mDisp->SetPage(page);
		  mFoundLangPage = 1;
		  fprintf(stderr, "Found subtitle page: %03x\n", page); // XXX
		}
	      }
	}
      }
      break;
    }
  }
}
