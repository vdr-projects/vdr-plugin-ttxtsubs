
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ttxtsubsfilter.h"
#include "teletext.h"

cTtxtSubsFilter::cTtxtSubsFilter(void)
  :
  mCount(0),
  mPages(0)
{
  int i;
  for(i = 0; i < 8; i++)
    mMagPage[i] = -1;
}


cTtxtSubsFilter::~cTtxtSubsFilter(void)
{
  FreePages();
}


void cTtxtSubsFilter::SetPages(size_t count, uint16_t *pages)
{
  FreePages();

  mPages = (uint16_t *) malloc(count * sizeof(uint16_t));

  memcpy((char *) mPages, (char *) pages, count * sizeof(uint16_t));

  mCount = count;

  if(0) {
    fprintf(stderr, "SetPages: %d, ", count);
    for(size_t i = 0; i < count; i++)
      fprintf(stderr, "%03x ", pages[i]);
    fprintf(stderr, "\n");
  }
}

// XXX IF MERGING PAGES FROM SEVERAL PIDS (which we currently don't)
// Need to insert a line Y0 if
// - there has been data from other streams inserted and the next line is not a Y0
// there has been data from other magazines and the next line for this mag is not a Y0
// indata and outdata are pointers to a ttxt data row buffers,
// if return value <> 0 outdata countains data to pass on
int cTtxtSubsFilter::Filter(char *indata, char *outdata)
{
  int mp;
  int mag; // X in ETSI EN 300 706
  int packet; // Y
  int result = 0;

  struct ttxt_data_field *d = (struct ttxt_data_field *) indata;

  mp = UNHAM_INV(d->mag_addr_ham[0], d->mag_addr_ham[1]);
  mag = mp & 0x7;
  packet = (mp >> 3) & 0x1f;

  if (0) { // XXX
    uint8_t buf[2];
    ham8_4byte(mp, buf);
    fprintf(stderr, "unham %02x %02x -> %02x, ham: %02x %02x\n",
	    invtab[d->mag_addr_ham[0]], invtab[d->mag_addr_ham[1]],
	    mp,
	    buf[0], buf[1]);
  }

  // packet 0, change page no in this magazine
  if(packet == 0) {
    uint8_t no;
    int newindex;

    no = UNHAM_INV(d->data[0], d->data[1]);
    
    mMagPage[mag] = newindex = Find(mCount, mPages, (mag << 8) | no);

    if(newindex != -1) {
      //fprintf(stderr, "Filter: %x%02x (%d)\n", mag, no, newindex); // XXX

      MakeY0(outdata, indata, mPages[newindex]);
      result = 1;
    }
  } else if(packet >= 1 && packet <= 25) {
    if(mMagPage[mag] != -1) {
      memcpy(outdata, indata, sizeof(ttxt_data_field));
      result = 1;
    }
  }

  return result;
}


void cTtxtSubsFilter::FreePages(void)
{
  mCount = 0;

  if(mPages) {
    free(mPages);
    mPages = NULL;
  }
}


int cTtxtSubsFilter::Find(int count, uint16_t *pages, uint16_t pageno){

  int i;
  for(i = 0; i < count; i++) {
    if(pages[i] == pageno)
      return i;
  }

  return -1;
}


// make a new Y0 line with the flags in 
void cTtxtSubsFilter::MakeY0(char *outdata, char *indata, uint16_t newpageno)
{
  struct ttxt_data_field *i = (struct ttxt_data_field *) indata;
  struct ttxt_data_field *o = (struct ttxt_data_field *) outdata;
  uint8_t hambuf[2];

  o->data_unit_id = 3; // EBU Teletxt subtitle data
  o->data_unit_length = 44;
  o->par_loff = 0xc0; // reserved bits + unknown line
  o->framing_code = 0xe4;

  // new magazine number (Y = 0)
  ham8_4byte((newpageno >> 8) & 0x7, hambuf);
  o->mag_addr_ham[0] = invtab[hambuf[0]];
  o->mag_addr_ham[1] = invtab[hambuf[1]];
  
  // new page number
  ham8_4byte(newpageno & 0xff, hambuf);
  o->data[0] = invtab[hambuf[0]];
  o->data[1] = invtab[hambuf[1]];

  // copy flags
  memcpy(o->data + 2, i->data + 2, 6);

  // new text
  unsigned char txtbuf[32];
  size_t txtlen;
  size_t n;

  txtlen = snprintf((char *) txtbuf, 32, "%03x", newpageno < 0x100 ? newpageno + 0x800 : newpageno);

  for(n = 0; n < txtlen; n++) {
    if(parity(txtbuf[n]))
      o->data[n + 8] = invtab[txtbuf[n] | 0x80]; // XXX wrong parity?
    else
      o->data[n + 8] = invtab[txtbuf[n]]; // XXX wrong parity?
  }
  for(; n < 32; n++)
    o->data[n + 8] = invtab[(unsigned char) ' ']; // space already has right parity
}
