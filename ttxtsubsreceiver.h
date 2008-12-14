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

class cRingBufferFrame;

struct encodedPTS {
  uint8_t valid;
  uint8_t data[5];
};

class cTtxtSubsReceiver : public cReceiver
{
 public:
  cTtxtSubsReceiver(tChannelID ChnId, int Ca, struct ttxtpidinfo *PI);
  virtual ~cTtxtSubsReceiver();

  // returns pointer buf if there is new data
  uint8_t *Get(uint8_t *buf, encodedPTS *pts = NULL);
  // wait for more data
  void Wait(void);

  void IndexPage(char ***lines, int *linecount);

 protected:
  virtual void Activate(bool On);
  virtual void Receive(uchar *Data, int Length);

  virtual void AddIndexInfo(char *lang, int type, uint16_t page);
  virtual void EndIndexInfo(void);

 private:
  cMutex mGetMutex;
  cCondVar mGetCond;
  cRingBufferFrame mRingBuf;
  struct ttxtpidinfo mPI;
  cTtxtSubsFilter mFilter;
  uint8_t mIndexPage[24][46];
  uint8_t mIndexPageLines;
  uint8_t mIndexPageCol;
  uint16_t mIndexPageNo;
  encodedPTS mPTS;
};
