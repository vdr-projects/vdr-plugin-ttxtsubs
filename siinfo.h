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

#define TTXT_INITIAL_PAGE 1
#define TTXT_SUBTITLE_PAGE 2
#define TTXT_ADDITIONAL_INFO_PAGE 3
#define TTXT_PROGRAMME_SCHEDULE_PAGE 4
#define TTXT_SUBTITLE_HEARING_IMPAIRED_PAGE 5

struct ttxtpageinfo {
  char lang[3];
  uint8_t type;
  uint8_t mag;
  uint8_t page;
};

struct ttxtpidinfo {
    uint16_t pid;
    int pagecount;
    struct ttxtpageinfo *i;
};

// XXX should be an object instead and use c++ vectors etc
struct ttxtinfo {
  int pidcount;
  struct ttxtpidinfo *p;
};

/*
 * find the ttxt_info in the PMT via the PAT, try first with the SID
 * and if that fails with the VPID
 * return <> 0 on error;
 */
int GetTtxtInfo(int card_no, const cChannel *c, struct ttxtinfo *info);

void FreeTtxtInfoData(struct ttxtinfo *info);
void DupTtxtInfo(struct ttxtinfo *in, struct ttxtinfo *out);

struct ttxtpidinfo *FindSubs(struct ttxtinfo *info, int *pid, int *pageno, char *lang);
