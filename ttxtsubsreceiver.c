
#include <stdlib.h>
#include <vdr/receiver.h>
#include <vdr/ringbuffer.h>

#include "siinfo.h"
#include "ttxtsubsfilter.h"
#include "ttxtsubsreceiver.h"
#include "teletext.h"

#define MAXINDEXPAGELINES 24

// ----- cTtxtSubsReceiver -----

cTtxtSubsReceiver::cTtxtSubsReceiver(int Ca, struct ttxtpidinfo *PI)
  :
  cReceiver(Ca, -1, 1, PI->pid),
  mGetMutex(),
  mGetCond(),
  mRingBuf(46 * 500, true),
  mPI(*PI),
  mIndexPageLines(0),
  mIndexPageCol(0),
  mIndexPageNo(0)
{
  int count = 0;
  uint16_t *pages = (uint16_t *) malloc(sizeof(uint16_t) * mPI.pagecount);

  // find a free page to put the index page on
  mIndexPageNo = 0x100;
  int again;
  do {
    again = 0;
    for(int i = 0; i < count; i++) {
      if(pages[i] == mIndexPageNo) {
	mIndexPageNo++;
	if((mIndexPageNo & 0x000f) == 0x000a)
	  mIndexPageNo += 6;
	again = 1;
      }
    }
  } while(again);

  // get pages to record
  for(int i = 0; i < mPI.pagecount; i++) {
    // only subtitle pages
    if(mPI.i[i].type != TTXT_SUBTITLE_PAGE &&
       mPI.i[i].type != TTXT_SUBTITLE_HEARING_IMPAIRED_PAGE)
      continue;
    
    pages[count] = (mPI.i[i].mag << 8) + mPI.i[i].page;
    
    AddIndexInfo(mPI.i[i].lang, mPI.i[i].type, pages[count]);
    
    count++;
  }

  mFilter.SetPages(count, pages);
  EndIndexInfo();
  
  free(pages);
}


cTtxtSubsReceiver::~cTtxtSubsReceiver()
{
  // get any waiting threads going
  mGetCond.Broadcast();
}


// returns pointer buf if there is new data
uint8_t *cTtxtSubsReceiver::Get(uint8_t *buf)
{
  const cFrame *f;

  f = mRingBuf.Get();
  if(f) {
    memcpy(buf, f->Data(), 46);
    mRingBuf.Drop(f);
    // fprintf(stderr, "cTtxtSubsReceiver::Get: returned data!\n");
    return buf;
  } else {
    return NULL;
  }
}


void cTtxtSubsReceiver::IndexPage(char ***lines, int *linecount)
{
  static char *lineptrs[MAXINDEXPAGELINES];

  for(int i = 0; i < mIndexPageLines; i++)
    lineptrs[i] = (char *) mIndexPage[i];

  *linecount = mIndexPageLines;
  *lines = lineptrs;
}


// wait for more data
void cTtxtSubsReceiver::Wait(void)
{
  mGetMutex.Lock();
  mGetCond.Wait(mGetMutex);
  mGetMutex.Unlock();
}


void cTtxtSubsReceiver::Activate(bool On)
{
}


// Take TS packets and break out the teletext data
// Buffer the data for processing in a separate thread
// XXX We should do some filtering here to avoid unneccessary load!
void cTtxtSubsReceiver::Receive(uchar *Data, int Length)
{
  int i;

  if(Length != 188) // should never happen
    return;

  if(Data[1] & 0x80) // transport_error_indicator
    return;

  // payload_unit_start_indicator
  for(i = (Data[1] & 0x40) ? 1 : 0; i < 4; i++) {
    char buf[46];

    if(0xff == Data[4 + i*46]) // stuffing data
      continue;

    if(mFilter.Filter((char *) Data + 4 + i*46, buf)) {
      // fprintf(stderr, "Forward Packet:\n");
      // print_line((char *) Data + 4 + i*46);
      // print_line(buf);

      cFrame *f = new cFrame((uchar *) buf, 46);
      mRingBuf.Put(f);
      mGetCond.Broadcast();
    }
  }
}

static void init_line(uint8_t *buf, uint8_t Y, uint8_t mag)
{
  struct ttxt_data_field *d = (struct ttxt_data_field *) buf;
  uint8_t magbuf[2];

  d->data_unit_id = 0x02; // EBU Teletxt non-subtitle data
  d->data_unit_length = 0x2c;
  d->par_loff = 0xc0; // reserved bits + unknown line
  d->framing_code = 0xe4;
  ham8_4byte((Y << 3) | (mag & 0x7), magbuf);
  d->mag_addr_ham[0] = invtab[magbuf[0]];
  d->mag_addr_ham[1] = invtab[magbuf[1]];
}

static void copy_inv_par(uint8_t *dest, uint8_t *src, int count)
{
  for(int i = 0; i < count; i++)
    if(parity(src[i]))
      dest[i] = invtab[src[i] | 0x80];
    else
      dest[i] = invtab[src[i]];
}

void cTtxtSubsReceiver::AddIndexInfo(char *lang, int type, uint16_t page)
{
  //fprintf(stderr, "AddIndexInfo: %c%c%c/%d/%03x\n", lang[0], lang[1], lang[2], type, page); // XXX

  if(mIndexPageLines == MAXINDEXPAGELINES) {
    fprintf(stderr, "cTtxtSubsReceiver::AddIndexInfo: Index page full!\n");
    return;
  }

  if(mIndexPageLines == 0) { // make Y0 and info text
    struct ttxt_data_field *d = (struct ttxt_data_field *) mIndexPage[0];
    uint8_t buf[40];
    size_t txtlen;

    init_line(mIndexPage[mIndexPageLines], mIndexPageLines, mIndexPageNo >> 8);

    d->data[0] = invtab[ham8_4nibble(mIndexPageNo & 0x000f)];
    d->data[1] = invtab[ham8_4nibble((mIndexPageNo >> 4) & 0x000f)];
    uint8_t ham_inv_zero = invtab[ham8_4nibble(0)];
    d->data[2] = ham_inv_zero;
    d->data[3] = ham_inv_zero;
    d->data[4] = ham_inv_zero;
    d->data[5] = ham_inv_zero;
    d->data[6] = ham_inv_zero;
    d->data[7] = ham_inv_zero;

    txtlen = snprintf((char *) buf, 32, "%03x", mIndexPageNo);
    copy_inv_par(d->data + 8, buf, txtlen);
    for(int i = txtlen; i < 32; i++)
      d->data[i+8] = invtab[' ']; // space already has correct parity

    //print_line((char *) mIndexPage[mIndexPageLines]); // XXX

    mIndexPageLines++;

    init_line(mIndexPage[mIndexPageLines], mIndexPageLines, mIndexPageNo >> 8);
    d = (struct ttxt_data_field *) mIndexPage[mIndexPageLines];
    char *header = "Subtitles Index Page";
    txtlen = strlen(header);
    copy_inv_par(d->data, (uint8_t *) header, txtlen);
    for(int i = txtlen; i < 40; i++)
      d->data[i] = invtab[' ']; // space already has correct parity
   
    //print_line((char *) mIndexPage[mIndexPageLines]); // XXX

    mIndexPageLines++;
  }

  struct ttxt_data_field *d = (struct ttxt_data_field *) mIndexPage[mIndexPageLines];
  uint8_t buf[10];

  if(mIndexPageCol == 0) {
    init_line(mIndexPage[mIndexPageLines], mIndexPageLines, mIndexPageNo >> 8);
    for(int i = 0; i < 40; i++)
      d->data[i+8] = invtab[' ']; // space already has correct parity
  }

  if(page < 0x100)
    page += 0x800;

  snprintf((char *) buf, 10, "%c%c%c%c%03x   ", lang[0], lang[1], lang[2],
	   type == TTXT_SUBTITLE_PAGE ? ' ' :
	   type == TTXT_SUBTITLE_HEARING_IMPAIRED_PAGE ? 'h' : 'x',
	   page);
  
  copy_inv_par(d->data+ mIndexPageCol*8, buf, 8);

  //print_line((char *) mIndexPage[mIndexPageLines]); // XXX

  mIndexPageCol++;
  
  if(mIndexPageCol == 5) {
    mIndexPageCol = 0;
    mIndexPageLines++;
  }
}

void cTtxtSubsReceiver::EndIndexInfo(void)
{
  if(mIndexPageCol != 0) {
    mIndexPageCol = 0;
    mIndexPageLines++;
  }
}
