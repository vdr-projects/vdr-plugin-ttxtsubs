#include "ttxtsubschannelsettings.h"

#include <vdr/channels.h>
#include <vdr/plugin.h>
#include <vdr/tools.h>

cTtxtSubsChannelSetting::cTtxtSubsChannelSetting(const cChannel *Channel, int PageMode, int PageNumber)
{
  channelid=Channel->GetChannelID();
  pagemode=PageMode;
  pagenumber=PageNumber;
}

bool cTtxtSubsChannelSetting::Parse(const char *s)
{
   bool result=false;
   char *buffer=NULL;
   int PageMode;
   int PageNumber;
   if (3 == sscanf(s, "%a[^,],%d,%d\n", &buffer, &PageMode, &PageNumber)) {
      tChannelID channelID=tChannelID::FromString(buffer);
      if (channelID.Valid()) {
        channelid=channelID;
        pagemode=PageMode;
        pagenumber=PageNumber;
        result=true;
      }
      else { // parse old format
        int sid;
        if (1 == sscanf(buffer, "%d", &sid)) {
          for (cChannel *ch = Channels.First(); ch; ch = Channels.Next(ch)) {
            if (!ch->GroupSep() && ch->Sid() == sid) {
              channelid=ch->GetChannelID();
              pagemode=PageMode;
              pagenumber=PageNumber;
              result=true;
              break;
            }
          }
        }
      }
   }
   free(buffer);
   return result;
}        

bool cTtxtSubsChannelSetting::Save(FILE *f)
{
  return fprintf(f, "%s,%d,%d\n", *channelid.ToString(), pagemode, pagenumber) > 0; 
}

