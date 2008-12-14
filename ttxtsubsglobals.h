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

class cPluginTtxtsubs;
class cMenuSetupTtxtsubs;
class cTtxtsubsPageMenu;
class cTtxtSubsChannelSettings;
class cTtxtSubsDisplay;

#define MAXLANGUAGES 5

class cTtxtsubsConf {
  friend class cPluginTtxtsubs;
  friend class cMenuSetupTtxtsubs;
  friend class cTtxtsubsPageMenu;
  friend class cTtxtSubsDisplay;

 public:
  cTtxtsubsConf(void)
    {
      mDoDisplay = 1;
      mRealDoDisplay =1;
      mDoRecord = 1;
      mMainMenuEntry = 0;
      mTextPos = 0;
      mBottomLB = 0;
      mBottomAdj = 0;
      mFrenchSpecial = 0;
      mLineSpacing = 0;
      mDvbSources = 0;
      mFontSize = 20;
      mFgColor = 1;
      mBgColor = 8;
      mCustomColor = 0xBB333333;
      memset(mLanguages, 0, sizeof(mLanguages));
      memset(mHearingImpaireds, 0, sizeof(mHearingImpaireds));
      mI18nLanguage = 0;
      mLiveDelay = 0;
      mReplayDelay = 0;
    }

 public:
  int doDisplay(void) {return mRealDoDisplay;}
  int doRecord(void) {return mDoRecord;}
  int mainMenuEntry(void) {return mMainMenuEntry;}
  int textPos(void) {return mTextPos;}
  int bottomLB(void) {return mBottomLB;}
  int bottomAdj(void) {return mBottomAdj;}
  int frenchSpecial(void) {return mFrenchSpecial;}
  int lineSpacing(void) {return mLineSpacing;}
  int dvbSources(void) {return mDvbSources;}
  int fontSize(void) {return mFontSize;}
  int fgColor(void) {return mFgColor;}
  int bgColor(void) {return mBgColor;}
  int customColor(void) {return mCustomColor;}
  char (*languages(void))[MAXLANGUAGES][2][4] {return &mLanguages;}
  int (*hearingImpaireds(void))[MAXLANGUAGES][2] {return &mHearingImpaireds;}

  int langChoise(const char *lang, const int HI);
  int i18nLanguage(void) {return mI18nLanguage;}
  int liveDelay(void) {return mLiveDelay;}
  int replayDelay(void) {return mReplayDelay;}

 protected:
  int mDoDisplay;
  int mRealDoDisplay;
  int mDoRecord;
  int mMainMenuEntry;
  int mTextPos;
  int mBottomLB;
  int mBottomAdj;
  int mFrenchSpecial;
  int mLineSpacing;
  int mDvbSources;
  int mFontSize;
  int mFgColor;
  int mBgColor;
  int mCustomColor;
  char mLanguages[MAXLANGUAGES][2][4];
  int mHearingImpaireds[MAXLANGUAGES][2];
  int mI18nLanguage;
  int mLiveDelay;
  int mReplayDelay;
};

extern cTtxtsubsConf globals;
    
