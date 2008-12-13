/*
 * ttxtsubsi18n.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: ttxtsubsi18n.c,v 1.1 2004/03/02 01:08:45 ragge Exp $
 */

#include "ttxtsubsi18n.h"
#include <stdio.h>

const tI18nPhrase Phrases[] =
{
  { "Teletext subtitles", // English
    "", // Deutsch
    "", // Slovenian
    "", // Italian
    "", // Dutch
    "", // Portuguese
    "", // French
    "", // Norwegian
    "Tekstitys (Teksti-TV)", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Textning (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalanian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Hide teletext subtitles",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Piilota tekstitys (Teksti-TV)",
    "", // TODO
    "", // TODO
    "", // TODO
    "Göm textning (Text-TV)",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Display teletext subtitles",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Näytä tekstitys (Teksti-TV)",
    "", // TODO
    "", // TODO
    "", // TODO
    "Visa textning (Text-TV)",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Position Teletext Subtitles for 4:3/Anamorph",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "4:3/Anamorfinen tekstitys (Teksti-TV)",
    "", // TODO
    "", // TODO
    "", // TODO
    "Textning för 4:3/anamorf (Text-TV)",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Position Teletext Subtitles for Letterbox",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Letterbox tekstitys (Teksti-TV)",
    "", // TODO
    "", // TODO
    "", // TODO
    "Textning för brevlådeformat (Text-TV)",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Display Subtitles",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Näytä tekstitykset",
    "", // TODO
    "", // TODO
    "", // TODO
    "Visa textning",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Record Subtitles",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Tallenna tekstitykset",
    "", // TODO
    "", // TODO
    "", // TODO
    "Spela in textning",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Main Menu Alternative",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Päävalikon toiminnot",
    "", // TODO
    "", // TODO
    "", // TODO
    "Huvudmenyalternativ",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Display on/off",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "aktivointi",
    "", // TODO
    "", // TODO
    "", // TODO
    "Visa textning på/av",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "4:3/Letterbox",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "kuvasuhde",
    "", // TODO
    "", // TODO
    "", // TODO
    "4:3/brevlådeformat",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Text Horizontal Position",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Vaakatasaus",
    "", // TODO
    "", // TODO
    "", // TODO
    "Horisontell position",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Left",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "vasen",
    "", // TODO
    "", // TODO
    "", // TODO
    "vänster",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Center",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "keskitä",
    "", // TODO
    "", // TODO
    "", // TODO
    "centrerad",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Right",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "oikea",
    "", // TODO
    "", // TODO
    "", // TODO
    "höger",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Text Vertical Position",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Pystytasaus",
    "", // TODO
    "", // TODO
    "", // TODO
    "Vertikal positon",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "4:3/Anamorph",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "4:3/anamorfinen",
    "", // TODO
    "", // TODO
    "", // TODO
    "4:3/anamorf",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Letterbox",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "letterbox",
    "", // TODO
    "", // TODO
    "", // TODO
    "brevlådeformat",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Text Vertical Adjust",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Pystytasauksen hienosäätö",
    "", // TODO
    "", // TODO
    "", // TODO
    "Vertikal justering",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Line Spacing Adjust",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Rivivälin hienosäätö",
    "", // TODO
    "", // TODO
    "", // TODO
    "Radavstånd",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Workaround for some French chns",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Ranskalainen silmänkääntötemppu",
    "", // TODO
    "", // TODO
    "", // TODO
    "Fix för vissa franska kanaler",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Language",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Tekstityskieli",
    "", // TODO
    "", // TODO
    "", // TODO
    "Språk",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Hearing Impaired",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "kuulorajoitteisille",
    "", // TODO
    "", // TODO
    "", // TODO
    "Nedsatt hörsel",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Text Color",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Kirjasimen väri",
    "", // TODO
    "", // TODO
    "", // TODO
    "Färg på text",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Background Color",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "Taustaväri",
    "", // TODO
    "", // TODO
    "", // TODO
    "Färg på bakgrund",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Black",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "musta",
    "", // TODO
    "", // TODO
    "", // TODO
    "svart",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "White",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "valkoinen",
    "", // TODO
    "", // TODO
    "", // TODO
    "vit",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Red",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "punainen",
    "", // TODO
    "", // TODO
    "", // TODO
    "röd",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Green",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "vihreä",
    "", // TODO
    "", // TODO
    "", // TODO
    "grön",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Yellow",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "keltainen",
    "", // TODO
    "", // TODO
    "", // TODO
    "gul",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Magenta",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "magenta",
    "", // TODO
    "", // TODO
    "", // TODO
    "magenta",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Blue",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "sininen",
    "", // TODO
    "", // TODO
    "", // TODO
    "blå",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Cyan",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "syaani",
    "", // TODO
    "", // TODO
    "", // TODO
    "cyan",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Grey",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "harmaa",
    "", // TODO
    "", // TODO
    "", // TODO
    "grå",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Transparent",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "läpinäkyvä",
    "", // TODO
    "", // TODO
    "", // TODO
    "transparent",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { "Background",
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "", // TODO
    "tausta",
    "", // TODO
    "", // TODO
    "", // TODO
    "bakgrund",
    "", // TODO
    "", // TODO
    "", // TODO
#if VDRVERSNUM >= 10300
    "", // TODO
#endif
  },
  { NULL }
};
