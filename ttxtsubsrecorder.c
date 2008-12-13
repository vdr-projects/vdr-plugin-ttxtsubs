
#include <stdint.h>
#include <stdlib.h>

#include <vdr/vdrttxtsubshooks.h>
#include <vdr/device.h>
#include <vdr/channels.h>
#include <vdr/receiver.h>
#include <vdr/ringbuffer.h>

#include "siinfo.h"
#include "ttxtsubsfilter.h"
#include "ttxtsubsrecorder.h"
#include "ttxtsubsreceiver.h"
#include "ttxtsubs.h"
#include "teletext.h"

#define MAXPACKETSIZE 20000

// ----- cTtxtSubsRecorder -----

cTtxtSubsRecorder::cTtxtSubsRecorder(cDevice *dev, const cChannel *ch, char *lang, int HI)
  :
  mDev(dev),
  mSid(ch->Sid()),
  mVid(ch->Vpid()),
  mTtxtinfo(NULL),
  mReceiver(NULL),
  mPacketBuffer(NULL)
{
  mTtxtinfo = (struct ttxtinfo *) malloc(sizeof(*mTtxtinfo));
  struct ttxtpidinfo *pi = NULL;
  int pid, page;

  if(GetTtxtInfo(dev->CardIndex(), ch->Sid(), ch->Vpid(), mTtxtinfo)) {
    fprintf(stderr, "cTtxtSubsRecorder::cTtxtSubsRecorder: GetTtxtSubtitleInfo error!\n");
  } else {
    pi = FindSubs(mTtxtinfo, lang, HI, &pid, &page);
    
    if(!pi && mTtxtinfo->pidcount > 0) {
      pi = &(mTtxtinfo->p[0]);
      fprintf(stderr, "Selected language not found, just recording first teletext pid found.\n");
    }

    if(pi) {
      mReceiver = new cTtxtSubsReceiver(ch->Ca(), pi);
      mPacketBuffer = (uint8_t *) malloc(MAXPACKETSIZE);
    } else {
      fprintf(stderr, "No teletext pid found, not recording any (obviously).\n");
    }
  }
}

cTtxtSubsRecorder::~cTtxtSubsRecorder(void)
{
  if(mReceiver) {
    delete mReceiver;
    mReceiver = NULL;
  }

  if(mTtxtinfo) {
    FreeTtxtInfoData(mTtxtinfo);
    free(mTtxtinfo);
  }

  if(mPacketBuffer)
    free(mPacketBuffer);
}

uint8_t *cTtxtSubsRecorder::GetPacket(uint8_t **outbuf, size_t *lenp)
{
  int done = 0;
  size_t len;
  uint8_t *b = (uint8_t *) mPacketBuffer;
  uint8_t line[46];

  *outbuf = NULL;

  if(!mReceiver)
    return *outbuf;

  len = 46; // skip PES header area

  if(mReceiver->Get(line)) {
    // if first line is a Y0, insert an index page
    // XXX This isn't really correct, we don't know if there is parallel magazine
    // transmission and a page in the index pages' mag is in progress...
    
    int mp;
    //int mag; // X in ETSI EN 300 706
    int packet; // Y
    
    struct ttxt_data_field *d = (struct ttxt_data_field *) line;
    
    mp = UNHAM_INV(d->mag_addr_ham[0], d->mag_addr_ham[1]);
    //mag = mp & 0x7;
    packet = (mp >> 3) & 0x1f;
    
    if(packet == 0) {
      char **iplines;
      int iplcount;
      mReceiver->IndexPage(&iplines, &iplcount);
      for(int i = 0; i < iplcount; i++) {
	memcpy(b + len, (char *) iplines[i], 46);
	len += 46;
      }
    }
    
    // insert the line
    memcpy(b + len, line, 46);
    len += 46;
  }

  // get the rest of the lines
  while((len < (MAXPACKETSIZE - 184)) && !done) {
    if(mReceiver->Get(b + len)) {
      len += 46;
    } else
      done = 1;
  }
  
  if(len > 46) { // we have data, make a PES packet according to ETSI EN 300 472
    // fill out packet with stuffing data to 184 byte boundary
    size_t stufflen = 184 - (len % 184);
    if(stufflen == 184)
      stufflen = 0;
    memset((char *) b + len, 0xff, stufflen);

    len += stufflen;

    // fill in header
    uint8_t header[] = {0x00, 0x00, 0x01, 0xbd, // private data stream 1
		       0x00, 0x00, // len
		       0x80, // 10 and flags
		       0x00, // PTS_DTS_flags and other flags
		       0x24}; // PES_header_data_length
    
    memcpy((char *) b, (char *) header, sizeof(header));
    b[4] = (len-6) >> 8;
    b[5] = (len-6) & 0xff;
    memset((char *) b + sizeof(header), 0xff, 45 - sizeof(header)); // add stuffing bytes
    b[45] = 0x1f; // EBU data, our payload type to indicate we have filtered data

    *outbuf = b;
    *lenp = len;
  }

#if 0
  if(*outbuf) { // XXX
    fprintf(stderr, "cTtxtSubsRecorder::GetPacket: len: %d\n", len);
    for(size_t i = 46; i < len; i +=46) {
      struct ttxt_data_field *d = (struct ttxt_data_field *) b + i;
      if(d->data_unit_id != 0xff)
	print_line((char *) b + i);
    }
  }
#endif

  return *outbuf;
}

void cTtxtSubsRecorder::DeviceAttach(void)
{
  if(mReceiver)
    mDev->AttachReceiver(mReceiver);
}

