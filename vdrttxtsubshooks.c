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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "vdrttxtsubshooks.h"

// XXX Really should be a list...
static cVDRTtxtsubsHookListener *gListener;

// ------ class cVDRTtxtsubsHookProxy ------

class cVDRTtxtsubsHookProxy : public cVDRTtxtsubsHookListener
{
 public:
  virtual void HideOSD(void) { if(gListener) gListener->HideOSD(); };
  virtual void ShowOSD(void) { if(gListener) gListener->ShowOSD(); };
  virtual void PlayerTeletextData(uint8_t *p, int length)
    { if(gListener) gListener->PlayerTeletextData(p, length); };
  virtual cTtxtSubsRecorderBase *NewTtxtSubsRecorder(cDevice *dev, const cChannel *ch)
    { if(gListener) return gListener->NewTtxtSubsRecorder(dev, ch); else return NULL; };
};


// ------ class cVDRTtxtsubsHookListener ------

cVDRTtxtsubsHookListener::~cVDRTtxtsubsHookListener()
{
  gListener = 0;
}

void cVDRTtxtsubsHookListener::HookAttach(void)
{
  gListener = this;
  //printf("cVDRTtxtsubsHookListener::HookAttach\n");
}

static cVDRTtxtsubsHookProxy gProxy;

cVDRTtxtsubsHookListener *cVDRTtxtsubsHookListener::Hook(void)
{
  return &gProxy;
}

