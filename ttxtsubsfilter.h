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

// ----- cTtxtSubsFilter -----

// Filters out the pages on one teletext stream 
class cTtxtSubsFilter
{
 public:
  cTtxtSubsFilter(void);
  virtual ~cTtxtSubsFilter(void);

  void SetPages(size_t count, uint16_t *pages);

  // indata and outdata are pointers to a ttxt data row buffers,
  // if return value <> 0 outdata countains data to pass on
  int Filter(char *indata, char *outdata);

 private:
  void FreePages(void);
  int Find(int count, uint16_t *pages, uint16_t pageno);
  void MakeY0(char *outdata, char *indata, uint16_t newpageno);

  int mCount;          // number of pages to filter out
  uint16_t *mPages;    // array of page numbers
  int mMagPage[8];     /* index into pages for each mag number, or -1 if
			  not currently collecting pages for that mag */
};

