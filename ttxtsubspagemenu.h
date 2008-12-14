#ifndef ___TTXTSUBSPAGEMENU_H
#define ___TTXTSUBSPAGEMENU_H

#include <vdr/config.h>
#include <vdr/osd.h>
#include <vdr/channels.h>
#include "ttxtsubschannelsettings.h"

class cTtxtsubsPageMenu : public cOsdMenu 
{
    private:
        cChannel *channel;
        cTtxtSubsChannelSetting *setting;
        int mPageNumber;
        int mPageMode;
        int origPageNumber;
        int origPageMode;
        int DoDisplay;
    public:
        cTtxtsubsPageMenu();
        virtual ~cTtxtsubsPageMenu();
        virtual eOSState ProcessKey(eKeys Key);
};

#endif //___TTXTSUBSPAGEMENU_H
