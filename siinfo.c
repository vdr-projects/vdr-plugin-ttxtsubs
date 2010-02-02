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

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/poll.h>
#include <time.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <map>

#include <vdr/device.h>
#include <vdr/dvbdevice.h>
#include <vdr/channels.h>

#include "linux/dvb/dmx.h"
#include "siinfo.h"

#include "ttxtsubsglobals.h"
#include "ttxtsubschannelsettings.h"

#define DESCR_TELETEXT 0x56
#define DESCR_DVBSUBTITLES 0x59


#define PACK  __attribute__ ((__packed__))


// ETSI 300468 6.2.40
struct PACK ttxt_descr {
  uint8_t tag;
  uint8_t length;
  struct PACK {
    uint8_t lang[3];
    uint8_t type_mag; // 5 bits type, 3 bits mag
    uint8_t page_no;
  } d[1];
};

struct PACK sect_header {
  uint8_t table_id;
  uint16_t syntax_len;
  uint16_t transport_stream_id;
  uint8_t ver_cur;
  uint8_t section_number;
  uint8_t last_section_number;
};

// H.222.0 2.4.4.3, Table 2-25
struct PACK PAT_sect {
  uint8_t table_id;
  uint16_t syntax_len;
  uint16_t transport_stream_id;
  uint8_t vers_curr;
  uint8_t sect_no;
  uint8_t last_sect;
  struct PACK {
    uint16_t program_number;
    uint16_t res_PMTPID;
  } d[1];
};

// H.222.0 2.4.4.8, Table 2-28
struct PACK PMT_stream {
  uint8_t stream_type;
  uint16_t res_PID;
  uint16_t res_ES_info_len;
  uint8_t descrs[2];
};

struct PACK PMT_sect {
  uint8_t table_id;
  uint16_t syntax_len;
  uint16_t program_number;
  uint8_t vers_curr;
  uint8_t sect_no;
  uint8_t last_sect;
  uint16_t res_pcr;
  uint16_t res_program_info_length;
  struct PMT_stream s;
};


void dump_hex(char *msg, uint8_t *p, int len)
{
  int i;
  printf("%s:", msg);
  for(i = 0; i < len; i++) {
    printf(" %02x", p[i]);
  }
  printf("\n");

  printf("%s:", msg);
  for(i = 0; i < len; i++) {
    printf(" %c", isprint(p[i]) ? p[i] : '.');
  }
  printf("\n");
}


static int SetSectFilt(int fd, uint16_t pid, uint8_t tnr, uint8_t mask)
{
  int ret;

  struct dmx_sct_filter_params p;
  
  memset(&p, 0, sizeof(p));
  
  p.filter.filter[0] = tnr;
  p.filter.mask[0] = mask;
  p.pid     = pid;
  p.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
  
  if ((ret = ioctl(fd, DMX_SET_FILTER, &p)) < 0)  
    perror("DMX SET FILTER:");

  return ret;
}


static int
read_timeout(int fd, void *buf, size_t count, int timeout_ms) {
  int ret = -1;
  struct pollfd pi;

  pi.fd = fd;
  pi.events = POLLIN | POLLERR | POLLHUP | POLLNVAL;

  ret = poll(&pi, 1, timeout_ms);
  if(ret < 0)
    return errno;
  if(ret == 0) { // timeout
    esyslog("ttxtsubs: Service Information read: timeout!");
    return -1;
  }

  if(pi.revents == POLLIN) {
    ret = read(fd, buf, count);
    return ret;
  } else
    return -1;
}

/*
 * PID - pid to collect on
 * table_id - table id to filter out (H.222.0 table 2-26)
 * sects - pointer to char *[256], points to array of table sections, initially empty
 * numsects - number of sections in table
 * returns - 0 if everything ok
 */
#define SECTSIZE 1024
static int CollectSections(int card_no, int pid, int table_id, char **sects, int *numsects)
{
  int fd;
  int ret = -1;
  int last_section = 0;
  int done = 0;
  char *p = NULL;
  int n;
  char name[100];
  time_t start_time;

  // printf("CollectSections %d/%d\n", pid, table_id); // XXXX

  snprintf(name, sizeof(name), "/dev/dvb/adapter%d/demux0", card_no);

  memset(sects, 0, sizeof(char*) * 256);
  
  if((fd = open(name, O_RDWR)) < 0){
    perror("DEMUX DEVICE 1: ");
    return -1;
  }
  //  printf("ttxtsubs: siinfo.c: checking pid %d\n", pid); // XXXX
  if(SetSectFilt(fd, pid, table_id, 0xff)) {
    ret = -1;
    goto bail;
  }

  start_time = time(NULL);

  do {
    struct sect_header *h;
    int i;

    if((start_time + 1) < time(NULL)) {
      ret = -1;
      done = 1;
      break;
    }

    if(p == NULL)
      p = (char *) malloc(SECTSIZE);

    n = read_timeout(fd, p, SECTSIZE, 250);

    if(n < 8)
      continue;

    h = (struct sect_header *) p;

    if(n != ((ntohs(h->syntax_len) & 0xfff) + 3)) {
      esyslog("ttxtsubs: bad section length: %x / %x!\n", n, ntohs(h->syntax_len) & 0xfff);
      continue;
    }

    if(!(h->ver_cur & 0x01)) // if not current
      continue;

    last_section = h->last_section_number;

    if(!sects[h->section_number]) { // section_number
      sects[h->section_number] = p;
      p = NULL;
    }

    for(i = 0; i <= last_section; i++) {
      if(!sects[i]) {
	break;
      }

      if(i == last_section) {
	*numsects = last_section + 1;
	ret = 0;
	done = 1;
      }
    }

  } while (!done);

 bail:
  close(fd);
  return ret;
}

// When using a full featured card with hw_sections=0, at frequency change
// there seems to sometimes be left one (or more?) section block from the
// previous channel somewhere in the pipe. Whe need to remove that.
static void DiscardBufferedSections(int card_no, uint16_t pid, int table_id)
{
  int fd;
  char name[100];
  int ret = -1;
  int n = 0;
  char buf[SECTSIZE];

  snprintf(name, sizeof(name), "/dev/dvb/adapter%d/demux0", card_no);

  if((fd = open(name, O_RDWR)) < 0){
    perror("DEMUX DEVICE 1: ");
    return;
  }

  if(SetSectFilt(fd, pid, table_id, 0xff)) {
    goto bail;
  }

  // first read one section
  read_timeout(fd, buf, SECTSIZE, 1000);

  // this loop doesn't seem to be needed
  do {
    struct pollfd pi;
    pi.fd = fd;
    pi.events = POLLIN | POLLERR | POLLHUP | POLLNVAL;

    ret = poll(&pi, 1, 0);
    if(ret > 0 && (pi.revents & POLLIN)) {
      // TODO: Do something more useful if read fails!
      if(read(fd, buf, SECTSIZE) < 0) { }
      n++;
    }
  } while (ret > 0);

  if(n)
    printf("\nttxtsubs: DiscardBufferedSections: Discarded %d extra buffered sections\n\n", n + 1); // XXX
 bail:
  close(fd);
}

static void FreeSects(char **sects)
{
  int i;

  for(i = 0; i < 256; i++) {
    if(sects[i]) {
      free(sects[i]);
      sects[i] = NULL;
    }
  }
}


static void addpageinfo(struct ttxtinfo *info, uint16_t pid, struct ttxt_descr *descr, int descr_index)
{
  struct ttxtpidinfo *pd;
  struct ttxtpageinfo *pa;

  if(!info->p) {
    info->p = (struct ttxtpidinfo *) malloc(sizeof(*(info->p)));
    memset((char *) info->p, 0, sizeof(*(info->p)));
    info->p[0].pid = pid;
    info->pidcount++;
  }

  if(pid != info->p[info->pidcount - 1].pid) {
    info->pidcount++;
    info->p = (struct ttxtpidinfo *) realloc(info->p, sizeof(info->p[0]) * info->pidcount);
    memset((char *) &(info->p[info->pidcount - 1]), 0, sizeof(info->p[0]));
    info->p[info->pidcount - 1].pid = pid;
  }

  pd = &(info->p[info->pidcount - 1]);

  pd->pagecount++;
  if(!pd->i) {
    pd->i = (struct ttxtpageinfo *) malloc(sizeof(pd->i[0]));
  } else {
    pd->i = (struct ttxtpageinfo *) realloc(pd->i, sizeof(pd->i[0]) * pd->pagecount);
  }
  memset((char *) &(pd->i[pd->pagecount - 1]), 0, sizeof(pd->i[0]));

  pa = &(pd->i[pd->pagecount - 1]);

  pa->lang[0] = descr->d[descr_index].lang[0];
  pa->lang[1] = descr->d[descr_index].lang[1];
  pa->lang[2] = descr->d[descr_index].lang[2];
  pa->type = descr->d[descr_index].type_mag >> 3;
  pa->mag = descr->d[descr_index].type_mag & 0x7;
  pa->page = descr->d[descr_index].page_no;

  if(globals.frenchSpecial()) {
    /* By some silly reason some French channels (Canal+ and CanalSatellite)
       announce subtitles with the page number in decimal, for example
       page 859 when they really are on page 889. */
    uint8_t newpage = pa->page;
    if(pa->page >= 0x50 && pa->page <= 0x59)
      newpage += 0x30;
    if(pa->page != newpage) {
      uint8_t mag = pa->mag;
      if(mag == 0)
	mag = 8;
      esyslog("ttxtsubs: Warning: Remapped page number %01x%02x to %01x%02x!\n",
	      mag, pa->page, mag, newpage);
      pa->page = newpage;
    }
  }
}


static int HasVPID(int vpid, char **pmtsects, int numsects)
{
  int found = 0;
  int i;

  for(i = 0; i < numsects && !found; i++) {
    struct PMT_sect *psect;
    char *sp;
    char *end;

    psect = (struct PMT_sect *) pmtsects[i];
    end = pmtsects[i] + (ntohs(psect->syntax_len) & 0x3ff) - 7;
    // skip extra program info
    sp = ((char *) &(psect->s)) + (ntohs(psect->res_program_info_length) & 0xfff);
    
    while(sp < end && !found) {
      struct PMT_stream *s = (struct PMT_stream *) sp;

      if(s->stream_type == 1 || s->stream_type == 2) { // Video stream
	if((ntohs(s->res_PID) & 0x1fff) == vpid) {
	  //printf("Found ttxt by vpid: %d/%d\n", s->stream_type, (ntohs(s->res_PID) & 0x1fff));
	  found = 1;
	  break;
	}
      }
      sp += (ntohs(s->res_ES_info_len) & 0xfff) + 5;
    }
  }  
  
  return found;
}

static void ExtractTtxtInfo(char **pmtsects, int numsects, struct ttxtinfo *info, int *foundinfo)
{
  int i;

  for(i = 0; i < numsects; i++) {
    struct PMT_sect *psect;
    char *sp;
    char *end;

    psect = (struct PMT_sect *) pmtsects[i];
    end = pmtsects[i] + (ntohs(psect->syntax_len) & 0x3ff) - 7;
    // skip extra program info
    sp = ((char *) &(psect->s)) + (ntohs(psect->res_program_info_length) & 0xfff);
    
    while(sp < end) {
      struct PMT_stream *s = (struct PMT_stream *) sp;

      if(s->stream_type == 6) { // PES private data
	uint8_t *descr;

	for(descr = s->descrs; descr < s->descrs + (ntohs(s->res_ES_info_len) & 0xfff); descr += descr[1] + 2) {
	  if(descr[0] == DESCR_TELETEXT) {
	    struct ttxt_descr *t = (struct ttxt_descr *) descr;
	    int i, count = t->length / sizeof(t->d[0]);
	    for(i = 0; i < count; i++) {
	      addpageinfo(info, ntohs(s->res_PID) & 0x1fff, t, i);
	      //printf("%c%c%c: type %d, page: %01x%02x\n", t->d[i].lang[0], t->d[i].lang[1], t->d[i].lang[2], t->d[i].type_mag >> 3, t->d[i].type_mag & 0x7,  t->d[i].page_no);
	      *foundinfo = 1;
	    }
	  }
	}
      }
      sp += (ntohs(s->res_ES_info_len) & 0xfff) + 5;
    }
  }  
}

/*
 * if vpid != 0, we first search if we can find the vpid in this PMT,
 * and if so we go get the ttxt pid
 * return <> 0 on error
 */
static int FindTtxtInfoInPMT(int card_no, int pid, int vpid, struct ttxtinfo *info, int *foundinfo)
{
  int ret = -1;
  char *pmtsects[256];
  int numsects;

  // printf("FindTtxtInfoInPMT pid: %d, vpid: %d\n", pid, vpid); // XXXX

  ret = CollectSections(card_no, pid, 0x02, pmtsects, &numsects);
  if(ret)
    goto bail;

  if(vpid != 0) {
    if(!HasVPID(vpid, pmtsects, numsects)) {
      goto bail;
    }
  }

  ExtractTtxtInfo(pmtsects, numsects, info, foundinfo);

 bail:
  FreeSects(pmtsects);
  return ret;
}


/*
 * find the ttxt_info in the PMT via the PAT, try first with the SID
 * and if that fails with the VPID
 * return <> 0 on error;
 */
int GetTtxtInfo(int card_no, const cChannel *c, struct ttxtinfo *info)
{
  int ret = -1;
  char *patsects[256];
  int numsects;
  int i;
  int j;
  uint16_t pmt_pid = 0;
  int foundinfo = 0;
  int retry;

  memset((char *) info, 0, sizeof(*info));

#if 1
  // <manual page selection patch>
  cTtxtSubsChannelSetting *cs = 
    TtxtSubsChannelSettings.Get(c);
    
  if (cs && cs->PageMode() == PAGE_MODE_MANUAL) {
    struct ttxt_descr descr;
 
    if (cs->PageNumber() > 0) {
      int temp = cs->PageNumber();
      if (temp>=800) temp-=800;
      char langs[MAXLANGUAGES][2][4];
      char hi[MAXLANGUAGES][2];
      //put as the first language the first one configured,
      //so that FindSubs will later accept it
      memcpy(langs,globals.languages(),sizeof(langs));
      memcpy(hi,globals.hearingImpaireds(),sizeof(hi));
      for (i = 0; i < MAXLANGUAGES; i++) {
        for (j = 0; j < 2 ; j++) {
          if (!langs[i][j][0]) continue;
         
          descr.d[0].lang[0] = langs[i][j][0];
          descr.d[0].lang[1] = langs[i][j][1];
          descr.d[0].lang[2] = langs[i][j][2];
          if (hi[i][j]) 
            descr.d[0].type_mag = (TTXT_SUBTITLE_HEARING_IMPAIRED_PAGE << 3) + (temp / 100); 
          else 
            descr.d[0].type_mag = (TTXT_SUBTITLE_PAGE << 3) + (temp / 100);

          temp = temp % 100;
          descr.d[0].page_no = ((temp / 10) << 4) + (temp % 10);
      
          addpageinfo(info, c->Tpid(), &descr, 0);
          return 0;
        }
      }
      //no language configured
      return -1;       
    }
  }
  if (cs && cs->PageMode() == PAGE_MODE_DISABLED) {
    return -1;
  }
  // </manual page selection patch>
#endif

  for(retry = 0; retry <= 1 && !foundinfo; retry++) { // XXX retry two times due to flaky pat scanning with hw_sections=0

    // printf("GetTtxtInfo A sid: %d, vpid: %d\n", sid, vpid); // XXXX

#if 1
    // firmware fixed now?
    DiscardBufferedSections(card_no, 0, 0);
#endif
    
    ret = CollectSections(card_no, 0, 0, patsects, &numsects);
    if(ret)
      goto bail;
    
    if(c->Sid() != 0) {
      int found;
      
      for(i = 0, found = 0; i < numsects && !found; i++) {
	int numdescrs;
	struct PAT_sect *s = (struct PAT_sect *) patsects[i];
	
	numdescrs = ((ntohs(s->syntax_len) & 0x3FF) - 7) / 4;
	
	for(j = 0; j < numdescrs && !found; j++) {
	  uint16_t pno = ntohs(s->d[j].program_number);
	  
	  if(pno == 0)
	    continue; // network pid
	  
	  if(pno == c->Sid()) {
	    pmt_pid = ntohs(s->d[j].res_PMTPID) & 0x1fff;
	    found = 1;
	  }
	}
      }
    }
    
    if(pmt_pid != 0) {
      // printf("GetTtxtInfo B pmt_pid: %d, vpid: %d\n", pmt_pid, vpid); // XXXX
      ret = FindTtxtInfoInPMT(card_no, pmt_pid, 0, info, &foundinfo);
    } else {
      // SID not found, try searching VID in all SIDS
      if(c->Vpid() != 0) {
	int done;
	for(i = 0, done = 0; i < numsects && !done; i++) {
	  int numdescrs;
	  struct PAT_sect *s = (struct PAT_sect *) patsects[i];
	  
	  numdescrs = ((ntohs(s->syntax_len) & 0x3FF) - 7) / 4;
	  
	  for(j = 0; j < numdescrs && !done; j++) {
	    uint16_t pno = ntohs(s->d[j].program_number);
	    
	    if(pno == 0)
	      continue; // network pid
	    
	    pmt_pid = ntohs(s->d[j].res_PMTPID) & 0x1fff;
	    
	    // printf("GetTtxtInfo C pmt_pid: %d, vpid: %d\n", pmt_pid, vpid); // XXXX
	    ret = FindTtxtInfoInPMT(card_no, pmt_pid, c->Vpid(), info, &foundinfo);
	    if(ret) {
	      done = 1;
	    }
	    if(foundinfo)
	      done = 1;
	  }
	}
      }
    }

    FreeSects(patsects);
  }

bail:
  return ret;
}


void FreeTtxtInfoData(struct ttxtinfo *info)
{
  int i;

  for(i = 0; i < info->pidcount; i++) {
    free((void *) info->p[i].i);
  }

  free((void *) info->p);
}


void DupTtxtInfo(struct ttxtinfo *in, struct ttxtinfo *out)
{
  int i;

  out->pidcount = in->pidcount;
  out->p = (struct ttxtpidinfo *) malloc(sizeof(out->p[0]) * in->pidcount);

  for(i = 0; i < in->pidcount; i++) {
    out->p[i] = in->p[i];
    out->p[i].i = (struct ttxtpageinfo *) malloc(sizeof(out->p[0].i[0]) * in->p[i].pagecount);
    memcpy((void *) out->p[i].i, (void *) in->p[i].i, sizeof(out->p[0].i[0]) * in->p[i].pagecount);
  }
}


struct ttxtpidinfo *FindSubs(struct ttxtinfo *info, int *pid, int *pageno, char *lang)
{
  struct ttxtpidinfo *foundPI = NULL;
  int foundChoise = 1000;

  // walk through all available languages and remember most preferred one
  for(int i = 0; i < info->pidcount; i++) {
    for(int j = 0; j < info->p[i].pagecount; j++) {
      if((info->p[i].i[j].type != TTXT_SUBTITLE_PAGE) &&
	 (info->p[i].i[j].type != TTXT_SUBTITLE_HEARING_IMPAIRED_PAGE))
	continue;

      int ch = globals.langChoise(info->p[i].i[j].lang,
	  info->p[i].i[j].type == TTXT_SUBTITLE_HEARING_IMPAIRED_PAGE);
      if(ch != -1) {
	if(ch < foundChoise) {
	  foundChoise = ch;
	  *pid = info->p[i].pid;
	  *pageno = info->p[i].i[j].mag * 0x100 + info->p[i].i[j].page;
	  strncpy(lang, info->p[i].i[j].lang, 3);
	  foundPI = &(info->p[i]);
	}
      }
    }
  }

  if(foundPI) {
    return foundPI;
  }

  if(info->pidcount == 0)
    isyslog("ttxtsubs: No teletext subtitles on channel.");
  else {
    isyslog("ttxtsubs: Wanted subtitle language(s) not found on channel, available languages:");
    for(int i = 0; i < info->pidcount; i++) {
      for(int j = 0; j < info->p[i].pagecount; j++) {
	int page = info->p[i].i[j].mag * 0x100 + info->p[i].i[j].page;
	int type = info->p[i].i[j].type;
	if(page < 0x100)
	  page += 0x800;
	isyslog("ttxtsubs:     %03x: %c%c%c %s", page, info->p[i].i[j].lang[0],
		info->p[i].i[j].lang[1], info->p[i].i[j].lang[2],
		type == TTXT_INITIAL_PAGE ? "(Initial Page (The teletext start page, not a subtitles page!))" :
		type == TTXT_SUBTITLE_PAGE ? "(Subtitles)" :
		type == TTXT_ADDITIONAL_INFO_PAGE ? "(Additional Info Page)" :
		type == TTXT_PROGRAMME_SCHEDULE_PAGE ? "(Programme Schedule Page)" :
		type == TTXT_SUBTITLE_HEARING_IMPAIRED_PAGE ? "(Hearing Impaired)" : "(Unknown type)");
      }
    }
  }

  *pid = 0;
  *pageno = -1;
  return NULL;
}
