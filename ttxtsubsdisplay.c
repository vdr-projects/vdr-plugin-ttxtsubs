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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

#include <vdr/osd.h>
#include <vdr/osdbase.h>
#include <vdr/thread.h>
#include <vdr/font.h>
#include <vdr/config.h>

#include "ttxtsubsglobals.h"
#include "ttxtsubsdisplay.h"
#include "utils.h"

// pageState
enum {
  invalid = 0,
  collecting,
  interimshow,
  finished
};

// --------------------

class cOSDSelfMemory {
 public:
  cOSDSelfMemory(void) : mActive(0) {}
  ~cOSDSelfMemory() {}
  
  void SetActive(int a)
    {
      cMutexLock lock(&mLock);
      mActive = a;
      if(a)
	mThr = pthread_self();
    }
  int IsMe(void)
    {
      cMutexLock lock(&mLock);
      pthread_t thr = pthread_self();
      if(mActive && pthread_equal(thr, mThr))
	return 1;
      else
	return 0;
    }

 private:
  cMutex mLock;
  int mActive;
  pthread_t mThr;
};

class cOSDSelfMemoryLock {
 public:
  cOSDSelfMemoryLock(cOSDSelfMemory *m) : mMem(m) { mMem->SetActive(1); }
  ~cOSDSelfMemoryLock() { mMem->SetActive(0); }
  
 private:
  cOSDSelfMemory *mMem;
};

static cOSDSelfMemory gSelfMem;

// --------------------

cTtxtSubsDisplay::cTtxtSubsDisplay(void)
  :
  mPageState(invalid),
  mMag(0),
  mNo(0),
  mDoDisplay(1),
  mOsd(NULL),
  mOsdLock(),
  mLastDataTime(NULL)
{
  memset(&page.data, 0, sizeof(page.data));
  mLastDataTime = (struct timeval *) calloc(1, sizeof(*mLastDataTime));
  mOsdFont = cFont::CreateFont(Setup.FontOsd, globals.mFontSize);
  if (!mOsdFont || !mOsdFont->Height())
     mOsdFont = cFont::GetFont(fontOsd);
}


cTtxtSubsDisplay::~cTtxtSubsDisplay(void)
{
  if(mLastDataTime)
    free(mLastDataTime);
  if(mOsd) {
    cOsd* tmp = mOsd;
    mOsd = NULL;
    delete tmp;
  }
  if(mOsdFont && (mOsdFont != cFont::GetFont(fontOsd)))
    delete mOsdFont;
}


void cTtxtSubsDisplay::SetPage(int Pageno)  // Pageno is 0x000 to 0x799
{
  mMag = (Pageno >> 8) & 0xF;
  mNo = Pageno & 0xFF;

  Clear();
}


void cTtxtSubsDisplay::Hide(void)
{
  if(gSelfMem.IsMe()) {
    //dprint("cTtxtSubsDisplay::Hide - Ignoring self induced hide!\n");
    return;
  }

  //dprint("cTtxtSubsDisplay::Hide\n");
  cMutexLock lock(&mOsdLock);
  mDoDisplay = 0;
  ClearOSD();
}


void cTtxtSubsDisplay::Show(void)
{
  if(gSelfMem.IsMe()) {
    //dprint("cTtxtSubsDisplay::Show - Ignoring self induced show!\n");
    return;
  }

  //dprint("cTtxtSubsDisplay::Show\n");
  cMutexLock lock(&mOsdLock);
  mDoDisplay = 1;
  ShowOSD();
}


void cTtxtSubsDisplay::Clear(void)
{
  mPageState = invalid;
  ClearOSD();
}


void cTtxtSubsDisplay::TtxtData(const uint8_t *Data, uint64_t sched_time)
{
  int mp;
  int mag; // X in ETSI EN 300 706
  int packet; // Y
  struct ttxt_data_field *d;

  // idle time - if we have been sitting on a half finished page for a while - show it
  if(Data == NULL && mPageState == collecting) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // add second diff to usecs
    tv.tv_usec += 1000000 * (tv.tv_sec - mLastDataTime->tv_sec);

    if((tv.tv_usec - mLastDataTime->tv_usec) > 500000) {
      //dprint("Interimshow!\n");
      mPageState = interimshow;
      ClearOSD();
      ShowOSD();
    }
  }

  if(Data == NULL)
    return;

  //print_line((char *) Data);

  d = (struct ttxt_data_field *) Data;

  mp = unham(invtab[d->mag_addr_ham[0]], invtab[d->mag_addr_ham[1]]);
  mag = mp & 0x7;
  packet = (mp >> 3) & 0x1f;

  //dprint("cTtxtSubsDisplay::TtxtData: mag:%d packet: %d\n", mag, packet); // XXX

  if(packet == 0) {
    int i, no;
    uint8_t fi[8]; // flags field inverted

    for(i = 0; i < 8; i++)
      fi[i] = invtab[d->data[i]];

    if(mag == mMag) { /* XXX: && ! magazine_serial */
      if(mPageState == collecting) {
	mPageState = finished;
	ClearOSD();
	ShowOSD();
      }
      if(mPageState == interimshow)
	mPageState = finished;
    }

    no = unham(fi[0], fi[1]);

    //dprint("cTtxtSubsDisplay::TtxtData: page:%d%02x, packet: %d\n", mag, no, packet); // XXX
    //print_line((char *) Data);

    if(mag == mMag && no == mNo) {
      //dprint("cTtxtSubsDisplay::TtxtData: page:%d%02x, packet: %d\n", mag, no, packet); // XXX
      page.mag = mag;
      page.no = no;
      page.flags = 0;
      page.national_charset = 0;
      
      if(fi[3] & 0x80) { // Erase Page
	page.flags |= erasepage;
	memset(&page.data, 0, sizeof(page.data)); // only if erasepage is set?
      }
      if(fi[5] & 0x20) // Newsflash
	page.flags |= newsflash;
      if(fi[5] & 0x80) // Subtitle
	page.flags |= subtitle;
      if(fi[6] & 0x02) // Suppress Header
	page.flags |= suppress_header;
      // if(fi[6] & 0x08) // Update Indicator
      // if(fi[6] & 0x20) // Interrupted Sequence
      if(fi[6] & 0x80) // Inhibit Display
	page.flags |= inhibit_display;
      // if(fi[7] & 0x02) // Magazine Serial
      
      page.national_charset = ((fi[7] & 0x80) >> 7) +
	((fi[7] & 0x20) >> 4) + ((fi[7] & 0x08) >> 1);
      
      if(mPageState != collecting) {
        int diff = sched_time - cTimeMs::Now();
        //printf("Got sched_time %llx, diff %d\n", sched_time, diff);
        if (diff > 10) cCondWait::SleepMs(diff);
      }

      mPageState = collecting;
      gettimeofday(mLastDataTime, NULL);

      for(i = 0; i < 32; i++)
	page.data[0][i] = invtab[d->data[i+8]];
    }
  } else if(mag == page.mag && packet < TTXT_DISPLAYABLE_ROWS && 
	    (mPageState == collecting || mPageState == interimshow)) {
    // mag == page.mag: The magazines can be sent interleaved
    int i;
    for(i = 0; i < 40; i++)
      page.data[packet][i] = invtab[d->data[i]];

    mPageState = collecting;
    gettimeofday(mLastDataTime, NULL);

    //dprint("row: %d ", packet);
    //dump_hex("", page.data[packet], 40);
  } else {
    // packets with national characters information: X/28/0 format 1, X/28/1, X/28/4, M/29/0 M/29/4, 
    if(packet == 28 || packet == 29) {
      //dprint("mag: %d, packet: %d, page: %02x, state: %d\n", page.mag, packet, page.no, mPageState);
    }
    
    //if(packet == 26) {
    //  dprint("mag: %d, packet: %d, page: %02x, state: %d\n", page.mag, packet, page.no, mPageState);
    //  dump_hex("", page.data[packet], 40);
    //}
  }
}


static char *
ttxt2la1(uint8_t *p, char *buf, int natopts)
{
  int i, j = 0, skip = 1;
  for(i = 0; i < 40; i++) {
    uint8_t c = p[i] & 0x7f;

    if(c == 0x0b) { // box begin
      skip = 0;
      if(j != 0) // if we have chars in buf
	buf[j++] = ' ';
      continue;
    }

    if(c == 0x0a) { // box end
      skip = 1;
      continue;
    }

    if(skip)
      continue;
    
    if(j == 0 && c == 20) // skip leading spaces
      continue;

    if(c >= 0x20) {
      uint16_t aux = ttxt_laG0_la1_char(0, natopts, c);
      if (aux & 0xff00)
         buf[j++] = (aux & 0xff00) >> 8;
      buf[j++] = aux & 0x00ff;
    }
  }

  while(j > 0 && buf[j-1] == ' ') // strip extra spaces
    j--;

  buf[j++] = '\0';
  if(strlen(buf))
    return buf;
  else
    return NULL;
}

#if 0

#define TEST_CENTER 0
#if TEST_CENTER
#define SCREENLEFT 0
#define SCREENWIDTH 720
#else
#define SCREENLEFT 125
#endif

#define TEST_169 0
#if TEST_169
#define SCREENBOT 480
#else
#define SCREENBOT 540
#endif

#if 0
#define ROWINCR 45
#define ROWH 36
#define TEXTY 4
#else
#define ROWINCR 43
#define ROWH 34
#define TEXTY 3
#endif
#define TEXTX 15
//#define SCREENTOP (SCREENBOT - MAXTTXTROWS * ROWINCR)
#define SCREENTOP 100

#endif

enum {
  SCREENLEFT = 0,
  SCREENRIGHT = 719,
  SCREENTOP = 150,
  SCREENBOTTOM = 575,
  
  SIDEMARGIN = 125,
  BOTNORM = 540,
  BOTLETTERBOX = 482,

  ROWINCR = 43,
  ROWH = 34,
  TEXTY = 3,
  TEXTX = 15
};

static tColor
getcolor(int color)
{ 
  switch (color)
    {
    case 0:  return clrBlack;
    case 1:  return clrWhite;
    case 2:  return clrRed;
    case 3:  return clrGreen;
    case 4:  return clrYellow;
    case 5:  return clrMagenta;
    case 6:  return clrBlue;
    case 7:  return clrCyan;
    case 8:  return globals.customColor();
    case 9:  return clrTransparent;
    default: return clrGray50;
    }
  return clrGray50;
}

void cTtxtSubsDisplay::ShowOSD(void)
{
  int i, y;
  int rowcount = 0;
  char buf[TTXT_DISPLAYABLE_ROWS][82];
  int bottom = globals.bottomAdj() + (globals.bottomLB() ? BOTLETTERBOX : BOTNORM);
  tArea areas[MAXOSDAREAS];
  int numAreas = 0;

  cOSDSelfMemoryLock selfmem(&gSelfMem);
  cMutexLock lock(&mOsdLock);

  if(!globals.mRealDoDisplay) {
    //dprint("NOT displaying subtitles because disabled!\n");
    return;
  }
 
  if(!mDoDisplay) {
    //dprint("NOT displaying subtitles because of other OSD activities!\n");
    return;
  }
  
  if(mPageState != interimshow && mPageState != finished) {
    //dprint("NOT displaying subtitles because page state: %d!\n", mPageState);
    return;
  }

  for(i = 1; i < 24; i++) {
    if(page.data[i][0] != 0) // anything on this line?
      if(ttxt2la1(page.data[i], buf[rowcount], page.national_charset))
	rowcount++;
  }

  if(mOsd) {
    cOsd* tmp = mOsd;
    mOsd = NULL;
    delete tmp;
  }

     mOsd = cOsdProvider::NewOsd(SCREENLEFT, SCREENTOP, 20); // level 20
     if(!mOsd) {
       //dprint("Error: cOsdProvider::NewOsd() returned NULL!\n");
       return;
       }

  if(rowcount > MAXTTXTROWS)
    rowcount = MAXTTXTROWS;
  y = bottom - SCREENTOP - ROWH - ((ROWINCR + globals.lineSpacing()) * (rowcount-1));

  if (Setup.AntiAlias) {
    // create only one osd area that's big enough for all rows
    int x1 = SCREENRIGHT+1, x2 = 0, y1 = SCREENBOTTOM+1, y2 = 0;
    for(i = 0; i < rowcount; i++) {
      int w = mOsdFont->Width(buf[i]) + 2 * TEXTX;
      int left = SIDEMARGIN;
      if(w % 4)
        w += 4 - (w % 4);
      switch(globals.textPos()) {
      case 1:
        left = (SCREENRIGHT - w) / 2;
        break;
      case 2:
        left = SCREENRIGHT - SIDEMARGIN - w;
        break;
      }
      if(x1 > left) 
        x1 = left;
      if(x2 < (left+w-1))
        x2 = left+w-1;
      if(y1 > y) 
        y1 = y;
      if(y2 < (y+ROWH-1))
        y2 = y+ROWH-1;
      y += (ROWINCR + globals.lineSpacing());
    }
    if((x1 >= x2) || (y1 >= y2)) // validate calculated area
      return;
    tArea area = {x1, y1, x2, y2, 8};
    areas[numAreas++] = area;
  } else {
    for(i = 0; i < rowcount; i++) {
      int w = 0;
      int left = SIDEMARGIN;
      w = mOsdFont->Width(buf[i]) + 2 * TEXTX;
      if(w % 4)
        w += 4 - (w % 4);
      switch(globals.textPos()) {
        case 1:
          left = (SCREENRIGHT - w) / 2;
          break;
        case 2:
          left = SCREENRIGHT - SIDEMARGIN - w;
          break;
        }
      tArea area = {left, y, left+w-1, y+ROWH-1, 2};
      areas[numAreas++] = area;
      y += (ROWINCR + globals.lineSpacing());
    }
  }
  if (mOsd->CanHandleAreas(areas, numAreas) != oeOk) {
    // try lower color depth
    if (Setup.AntiAlias) {
      for(i = 0; i < numAreas; i++) areas[numAreas++].bpp = 2;
      if (mOsd->CanHandleAreas(areas, numAreas) != oeOk) {
        dprint("ttxtsubs: OSD Cannot handle areas (error code: %d) - try to enlarge the line spacing!\n", mOsd->CanHandleAreas(areas, numAreas));
        return;
        }
      }
    }
  mOsd->SetAreas(areas, numAreas);

  for (i = 0; i < numAreas; i++) {
    mOsd->DrawRectangle(areas[i].x1, areas[i].y1, areas[i].x2, areas[i].y2, clrTransparent);
    }

  y = bottom - SCREENTOP - ROWH - ((ROWINCR + globals.lineSpacing()) * (rowcount-1));
  for(i = 0; i < rowcount; i++) {
    int w = 0;
    int left = SIDEMARGIN;
    w = mOsdFont->Width(buf[i]) + 2 * TEXTX;
    if(w % 4)
      w += 4 - (w % 4);
    switch(globals.textPos()) {
      case 1:
        left = (SCREENRIGHT - w) / 2;
        break;
      case 2:
        left = SCREENRIGHT - SIDEMARGIN - w;
        break;
      }
    mOsd->DrawRectangle(left, y, left + w, y + ROWH, getcolor(globals.bgColor()));
    mOsd->DrawText(left + TEXTX, y + TEXTY, buf[i], getcolor(globals.fgColor()), getcolor(globals.bgColor()), mOsdFont);
    //dprint("%d/%d (%d,%d) (%d,%d): %s\n", i, rowcount-1, areas[i].x1, areas[i].y1, left + TEXTX, y + TEXTY, buf[i]);
    y += (ROWINCR + globals.lineSpacing());
    }
  mOsd->Flush();
}


void cTtxtSubsDisplay::ClearOSD(void)
{
  // dprint("\nClearOSD!\n");
  cOSDSelfMemoryLock selfmem(&gSelfMem);
  cMutexLock lock(&mOsdLock);

  if(mOsd) {
    cOsd* tmp = mOsd;
    mOsd = NULL;
    delete tmp;
  }
}
