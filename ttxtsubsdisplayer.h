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

#include <vdr/receiver.h>
#include <vdr/ringbuffer.h>

class cTtxtSubsDisplay;

class cTtxtSubsDisplayer : public cThread {
 public:
  cTtxtSubsDisplayer(int textpage);
  ~cTtxtSubsDisplayer(void);
  void ShowDisplay(void);
  void HideDisplay(void);

 protected:
  virtual void Action(void);

 protected:
  cTtxtSubsDisplay *mDisp;
  cMutex mGetMutex;
  cCondVar mGetCond;
  cRingBufferFrame mRingBuf;
  int mRun;
};

class cTtxtSubsLiveReceiver : public cReceiver, public cTtxtSubsDisplayer {
 public:
  cTtxtSubsLiveReceiver(tChannelID ChnId, int Pid, int textpage);
  ~cTtxtSubsLiveReceiver(void);

 protected:
  virtual void Activate(bool On);
  virtual void Receive(uchar *Data, int Length);
};

class cTtxtSubsPlayer : public cTtxtSubsDisplayer {
 public:
  cTtxtSubsPlayer(int backup_textpage);
  virtual void PES_data(uchar *Data, int Length, bool IsPesRecording);

 private:
  void SearchLanguagePage(uint8_t *p, int len);
  
  int mHasFilteredStream;
  int mFoundLangPage;
  int mLangChoise;
  int mLangInfoState;
};
