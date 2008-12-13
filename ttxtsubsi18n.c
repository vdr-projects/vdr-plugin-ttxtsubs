/*
 * ttxtsubsi18n.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: ttxtsubsi18n.c,v 1.6 2004/05/31 19:12:22 ragge Exp $
 */

#include "ttxtsubsi18n.h"
#include <stdio.h>

const tI18nPhrase Phrases[] =
{
  { "Teletext subtitles", // English
    "Videotextuntertitel", // German - Tobias Grimm 2004-05-21
    "", // Slovenian
    "Sottotitoli televideo", // Italian - Sean Carlos 2004-03-07
    "", // Dutch
    "", // Portuguese
    "Sous-titrage télétexte", // French - Stéphane Esté-Gracias 2004-03-02
    "", // Norwegian
    "Tekstitys (Teksti-TV)", // Finnish - Rolf Ahrenberg 2004-03-01
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
  { "Hide teletext subtitles", // English
    "Verstecke Videotextuntertitel", // German
    "", // Slovenian
    "Nascondi sottotitoli televideo", // Italian
    "", // Dutch
    "", // Portuguese
    "Masquer le sous-titrage", // French
    "", // Norwegian
    "Piilota tekstitys (Teksti-TV)", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Göm textning (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Display teletext subtitles", // English
    "Zeige Videotextuntertitel", // German
    "", // Slovenian
    "Visualizza sottotitoli televideo", // Italian
    "", // Dutch
    "", // Portuguese
    "Afficher le sous-titrage", // French
    "", // Norwegian
    "Näytä tekstitys (Teksti-TV)", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Visa textning (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Position Teletext Subtitles for 4:3/Anamorph", // English
    "Positioniere Videotext-Untertitel für 4:3/Anamorph", // German
    "", // Slovenian
    "Posizionare sottotitoli per 4:3/anamorfico", // Italian
    "", // Dutch
    "", // Portuguese
    "Position du sous-titrage pour 4:3/Anamorph", // French
    "", // Norwegian
    "4:3/Anamorfinen tekstitys (Teksti-TV)", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Textning för 4:3/anamorf (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Position Teletext Subtitles for Letterbox", // English
    "Positioniere Videotext-Untertitel für Letterbox", // German
    "", // Slovenian
    "Posizionare sottotitoli per Letterbox", // Italian
    "", // Dutch
    "", // Portuguese
    "Position du sous-titrage pour Letterbox", // French
    "", // Norwegian
    "Letterbox tekstitys (Teksti-TV)", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Textning för brevlådeformat (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Display Subtitles", // English
    "Zeige Untertitel", // German
    "", // Slovenian
    "Visualizza sottotitoli", // Italian
    "", // Dutch
    "", // Portuguese
    "Afficher le sous-titrage", // French
    "", // Norwegian
    "Näytä tekstitykset", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Visa textning", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Record Subtitles", // English
    "Nimm Untertitel auf", // German
    "", // Slovenian
   "Registra sottotitoli", // Italian
    "", // Dutch
    "", // Portuguese
    "Enregistrer le sous-titrage", // French
    "", // Norwegian
    "Tallenna tekstitykset", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Spela in textning", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Main Menu Alternative", // English
    "Hauptmenu Alternative", // German
    "", // Slovenian
    "Alternativa menù principale", // Italian
    "", // Dutch
    "", // Portuguese
    "Choix du menu principal", // French
    "", // Norwegian
    "Päävalikon toiminnot", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Huvudmenyalternativ", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Display on/off", // English
    "Anzeige ein/aus", // German
    "", // Slovenian
    "Visualizza sì/no", // Italian
    "", // Dutch
    "", // Portuguese
    "Affichage", // French
    "", // Norwegian
    "aktivointi", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Visa textning på/av", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "4:3/Letterbox", // English
    "4:3/Letterbox",
    "", // Slovenian
    "4:3/Letterbox", // Italian
    "", // Dutch
    "", // Portuguese
    "4:3/Letterbox", // French
    "", // Norwegian
    "kuvasuhde", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "4:3/brevlådeformat", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Text Horizontal Position", // English
    "Horizontale Textposition", // German
    "", // Slovenian
    "Posizione testo orizzontale", // Italian
    "", // Dutch
    "", // Portuguese
    "Position horizontale du texte", // French
    "", // Norwegian
    "Vaakatasaus", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Horisontell position", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Left", // English
    "Links", // German
    "", // Slovenian
    "Sinistra", // Italian
    "", // Dutch
    "", // Portuguese
    "Gauche", // French
    "", // Norwegian
    "vasen", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "vänster", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Center", // English
    "Mitte", // German
    "", // Slovenian
    "Centro", // Italian
    "", // Dutch
    "", // Portuguese
    "Centre", // French
    "", // Norwegian
    "keskitä", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "centrerad", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Right", // English
    "Rechts", // German
    "", // Slovenian
    "Destra", // Italian
    "", // Dutch
    "", // Portuguese
    "Droite", // French
    "", // Norwegian
    "oikea", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "höger", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Text Vertical Position", // English
    "Vertikale Textposition", // German
    "", // Slovenian
    "Posizione testo verticale", // Italian
    "", // Dutch
    "", // Portuguese
    "Position verticale du texte", // French
    "", // Norwegian
    "Pystytasaus", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Vertikal positon", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "4:3/Anamorph", // English
    "4:3/Anamorph", // German
    "", // Slovenian
    "4:3/anamorfico", // Italian
    "", // Dutch
    "", // Portuguese
    "4:3/Anamorph", // French
    "", // Norwegian
    "4:3/anamorfinen", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "4:3/anamorf", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Letterbox", // English
    "Letterbox", // German
    "", // Slovenian
    "Letterbox", // Italian
    "", // Dutch
    "", // Portuguese
    "Letterbox", // French
    "", // Norwegian
    "letterbox", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "brevlådeformat", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Text Vertical Adjust", // English
    "Text vertikal verschieben", // German
    "", // Slovenian
    "Correzione testo verticale", // Italian
    "", // Dutch
    "", // Portuguese
    "Ajustement vertical du texte", // French
    "", // Norwegian
    "Pystytasauksen hienosäätö", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Vertikal justering", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Line Spacing Adjust", // English
    "Zeilenabstand einstellen", // German
    "", // Slovenian
    "Correzione interlinea", // Italian
    "", // Dutch
    "", // Portuguese
    "Espacement des lignes", // French
    "", // Norwegian
    "Rivivälin hienosäätö", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Radavstånd", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Workaround for some French chns", // English
    "Workaround für französische Kanäle", // German
    "", // Slovenian
    "Correzione per canali francesi", // Italian
    "", // Dutch
    "", // Portuguese
    "Contournement chaînes françaises", // French
    "", // Norwegian
    "Ranskalainen silmänkääntötemppu", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Fix för vissa franska kanaler", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Language", // English
    "Sprache", // German
    "", // Slovenian
    "Lingua", // Italian
    "", // Dutch
    "", // Portuguese
    "Langues", // French
    "", // Norwegian
    "Tekstityskieli", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Språk", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Hearing Impaired", // English
    "Gehörlose", // German
    "", // Slovenian
    "Non udente", // Italian
    "", // Dutch
    "", // Portuguese
    "Malentendant", // French
    "", // Norwegian
    "kuulorajoitteisille", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Nedsatt hörsel", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Text Color", // English
    "Textfarbe", // German
    "", // Slovenian
    "Colore testo", // Italian
    "", // Dutch
    "", // Portuguese
    "Couleur du text", // French
    "", // Norwegian
    "Kirjasimen väri", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Färg på text", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Background Color", // English
    "Hintergrundfarbe", // German
    "", // Slovenian
     "Colore sfondo", // Italian
    "", // Dutch
    "", // Portuguese
    "Couleur du fond", // French
    "", // Norwegian
    "Taustaväri", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "Färg på bakgrund", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Black", // English
    "Schwarz", // German
    "", // Slovenian
    "Nero", // Italian 
    "", // Dutch
    "", // Portuguese
    "Noir", // French
    "", // Norwegian
    "musta", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "svart", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "White", // English
    "Weiss", // German
    "", // Slovenian
    "Bianco", // Italian
    "", // Dutch
    "", // Portuguese
    "Blanc", // French
    "", // Norwegian
    "valkoinen", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "vit", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Red", // English
    "Rot", // German
    "", // Slovenian
    "Rosso", // Italian
    "", // Dutch
    "", // Portuguese
    "Rouge", // French
    "", // Norwegian
    "punainen", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "röd", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Green", // English
    "Grün", // German
    "", // Slovenian
    "Verde", // Italian
    "", // Dutch
    "", // Portuguese
    "Vert", // French
    "", // Norwegian
    "vihreä", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "grön", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Yellow", // English
    "Gelb", // German
    "", // Slovenian
    "Giallo", // Italian
    "", // Dutch
    "", // Portuguese
    "Jaune", // French
    "", // Norwegian
    "keltainen", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "gul", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Magenta", // English
    "Magenta", // German
    "", // Slovenian
    "Magenta", // Italian
    "", // Dutch
    "", // Portuguese
    "Magenta", // French
    "", // Norwegian
    "magenta", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "magenta", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Blue", // English
    "Blau", // German
    "", // Slovenian
    "Azzurro", // Italian
    "", // Dutch
    "", // Portuguese
    "Bleu", // French
    "", // Norwegian
    "sininen", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "blå", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Cyan", // English
    "Türkis", // German
    "", // Slovenian
    "Ciano", // Italian
    "", // Dutch
    "", // Portuguese
    "Cyan", // French
    "", // Norwegian
    "syaani", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "cyan", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Grey", // English
    "Grau", // German
    "", // Slovenian
    "Grigio", // Italian
    "", // Dutch
    "", // Portuguese
    "Gris", // French
    "", // Norwegian
    "harmaa", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "grå", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Transparent", // English
    "Transparent", // German
    "", // Slovenian
    "Trasparente", // Italian
    "", // Dutch
    "", // Portuguese
    "Transparent", // French
    "", // Norwegian
    "läpinäkyvä", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "transparent", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { "Background", // English
    "Hintergrund", // German
    "", // Slovenian
    "Sfondo", // Italian
    "", // Dutch
    "", // Portuguese
    "Fond", // French
    "", // Norwegian
    "tausta", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "bakgrund", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
#if VDRVERSNUM >= 10300
    "", // Russian
#endif
  },
  { NULL }
};
