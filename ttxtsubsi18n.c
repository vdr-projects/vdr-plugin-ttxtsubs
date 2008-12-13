/*
 * ttxtsubsi18n.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: ttxtsubsi18n.c,v 1.6 2004/05/31 19:12:22 ragge Exp $
 */

#include "ttxtsubsi18n.h"
#include <stdio.h>

const tI18nPhrase Phrases[] = {
  { "Teletext subtitles", // English
    "Videotextuntertitel", // German - Tobias Grimm 2004-05-21
    "", // Slovenian
    "Sottotitoli televideo", // Italian - Sean Carlos 2004-03-07
    "", // Dutch
    "", // Portuguese
    "Sous-titrage tÈlÈtexte", // French - StÈphane EstÈ-Gracias 2004-03-02
    "", // Norwegian
    "Tekstitys (Teksti-TV)", // Finnish - Rolf Ahrenberg 2004-03-01
    "", // Polish
    "SubtÌtulos teletexto", // Spanish
    "", // Greek
    "Textning (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "SubtÌtols teletext", // Catalanian
    "¬’€’‚’⁄·‚-·„—‚ÿ‚‡Î", // Russian - Vyacheslav Dikonov 2004-xx-xx
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Esconde subtÌtulos teletexto", // Spanish
    "", // Greek
    "Gˆm textning (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "Amaga subtÌtols teletext", // Catalenian
    "¡⁄‡Î‚Ï ·„—‚ÿ‚‡Î", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Display teletext subtitles", // English
    "Zeige Videotextuntertitel", // German
    "", // Slovenian
    "Visualizza sottotitoli televideo", // Italian
    "", // Dutch
    "", // Portuguese
    "Afficher le sous-titrage", // French
    "", // Norwegian
    "N‰yt‰ tekstitys (Teksti-TV)", // Finnish
    "", // Polish
    "Visualiza subtÌtulos teletexto", // Spanish
    "", // Greek
    "Visa textning (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "Visualitza subtÌtols teletext", // Catalenian
    "øﬁ⁄–◊Î“–‚Ï ·„—‚ÿ‚‡Î", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Position Teletext Subtitles for 4:3/Anamorph", // English
    "Positioniere Videotext-Untertitel f¸r 4:3/Anamorph", // German
    "", // Slovenian
    "Posizionare sottotitoli per 4:3/anamorfico", // Italian
    "", // Dutch
    "", // Portuguese
    "Position du sous-titrage pour 4:3/Anamorph", // French
    "", // Norwegian
    "4:3/Anamorfinen tekstitys (Teksti-TV)", // Finnish
    "", // Polish
    "Posiciona subtÌtulos teletexto para 4:3/anamÛrfico", // Spanish
    "", // Greek
    "Textning fˆr 4:3/anamorf (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "Posiciona subtÌtols teletext per a 4:3/anamÚrfic", // Catalenian
    "¡„—‚ÿ‚‡Î “ ⁄–‘‡’ 4:3", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Position Teletext Subtitles for Letterbox", // English
    "Positioniere Videotext-Untertitel f¸r Letterbox", // German
    "", // Slovenian
    "Posizionare sottotitoli per Letterbox", // Italian
    "", // Dutch
    "", // Portuguese
    "Position du sous-titrage pour Letterbox", // French
    "", // Norwegian
    "Letterbox tekstitys (Teksti-TV)", // Finnish
    "", // Polish
    "PosiciÛna subtÌtulos teletexto para Letterbox", // Spanish
    "", // Greek
    "Textning fˆr brevlÂdeformat (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "Posiciona subtÌtols teletext per a Letterbox", // Catalenian
    "¡„—‚ÿ‚‡Î “ ⁄–‘‡’ Ëÿ‡ﬁ⁄ﬁÌ⁄‡–››ﬁ”ﬁ ‰ÿ€Ï‹–", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Display Subtitles", // English
    "Zeige Untertitel", // German
    "", // Slovenian
    "Visualizza sottotitoli", // Italian
    "", // Dutch
    "", // Portuguese
    "Afficher le sous-titrage", // French
    "", // Norwegian
    "N‰yt‰ tekstitykset", // Finnish
    "", // Polish
    "Visualiza subtÌtulos", // Spanish
    "", // Greek
    "Visa textning", // Swedish
    "", // Romanian
    "", // Hungarian
    "Visualitza subtÌtols", // Catalenian
    "øﬁ⁄–◊Î“–‚Ï ·„—‚ÿ‚‡Î", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Graba subtÌtulos", // Spanish
    "", // Greek
    "Spela in textning", // Swedish
    "", // Romanian
    "", // Hungarian
    "Grava subtÌtols", // Catalenian
    "∑–ﬂÿ·Î“–‚Ï ·„—‚ÿ‚‡Î", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Main Menu Alternative", // English
    "Hauptmenu Alternative", // German
    "", // Slovenian
    "Alternativa men˘ principale", // Italian
    "", // Dutch
    "", // Portuguese
    "Choix du menu principal", // French
    "", // Norwegian
    "P‰‰valikon toiminnot", // Finnish
    "", // Polish
    "Alternativa del men˙ principal", // Spanish
    "", // Greek
    "Huvudmenyalternativ", // Swedish
    "", // Romanian
    "", // Hungarian
    "Alternativa del men˙ principal", // Catalenian
    "∫ﬁ‹–›‘– “ ”€–“›ﬁ‹ ‹’›Ó", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Display on/off", // English
    "Anzeige ein/aus", // German
    "", // Slovenian
    "Visualizza sÏ/no", // Italian
    "", // Dutch
    "", // Portuguese
    "Affichage", // French
    "", // Norwegian
    "aktivointi", // Finnish
    "", // Polish
    "Visualiza sÌ/no", // Spanish
    "", // Greek
    "Visa textning pÂ/av", // Swedish
    "", // Romanian
    "", // Hungarian
    "Visualitza si/no", // Catalenian
    "“⁄€ÓÁÿ‚Ï/“Î⁄€ÓÁÿ‚Ï", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "4:3/Letterbox", // Spanish
    "", // Greek
    "4:3/brevlÂdeformat", // Swedish
    "", // Romanian
    "", // Hungarian
    "4:3/Letterbox", // Catalenian
    "4:3/Ëÿ‡ﬁ⁄ÿŸ Ì⁄‡–›", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "¥PosiciÛn horizontal del texto", // Spanish
    "", // Greek
    "Horisontell position", // Swedish
    "", // Romanian
    "", // Hungarian
    "PosiciÛ horitzontal del text ", // Catalenian
    "¿–◊‹’È’›ÿ’ ‚’⁄·‚– ﬂﬁ ”ﬁ‡ÿ◊ﬁ›‚–€ÿ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Izquierda", // Spanish
    "", // Greek
    "v‰nster", // Swedish
    "", // Romanian
    "", // Hungarian
    "Esquerra", // Catalenian
    "·€’“–", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Center", // English
    "Mitte", // German
    "", // Slovenian
    "Centro", // Italian
    "", // Dutch
    "", // Portuguese
    "Centre", // French
    "", // Norwegian
    "keskit‰", // Finnish
    "", // Polish
    "Centro", // Spanish
    "", // Greek
    "centrerad", // Swedish
    "", // Romanian
    "", // Hungarian
    "Centre", // Catalenian
    "“ Ê’›‚‡’", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Derecha", // Spanish
    "", // Greek
    "hˆger", // Swedish
    "", // Romanian
    "", // Hungarian
    "Dreta", // Catalenian
    "·ﬂ‡–“–", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "PosiciÛn vertical del texto", // Spanish
    "", // Greek
    "Vertikal positon", // Swedish
    "", // Romanian
    "", // Hungarian
    "PosiciÛn vertical del text", // Catalenian
    "¿–◊‹’È’›ÿ’ ‚’⁄·‚– ﬂﬁ “’‡‚ÿ⁄–€ÿ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "4:3/AnamÛrfico", // Spanish
    "", // Greek
    "4:3/anamorf", // Swedish
    "", // Romanian
    "", // Hungarian
    "4:3/AnamÚrfic", // Catalenian
    "4:3", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Letterbox", // Spanish
    "", // Greek
    "brevlÂdeformat", // Swedish
    "", // Romanian
    "", // Hungarian
    "Letterbox", // Catalenian
    "Ëÿ‡ﬁ⁄ÿŸ Ì⁄‡–›", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Text Vertical Adjust", // English
    "Text vertikal verschieben", // German
    "", // Slovenian
    "Correzione testo verticale", // Italian
    "", // Dutch
    "", // Portuguese
    "Ajustement vertical du texte", // French
    "", // Norwegian
    "Pystytasauksen hienos‰‰tˆ", // Finnish
    "", // Polish
    "Ajuste vertical del texto", // Spanish
    "", // Greek
    "Vertikal justering", // Swedish
    "", // Romanian
    "", // Hungarian
    "Ajustament vertical del text", // Catalenian
    "≤’‡‚ÿ⁄–€Ï›ÎŸ ·‘“ÿ” ‚’⁄·‚–", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Line Spacing Adjust", // English
    "Zeilenabstand einstellen", // German
    "", // Slovenian
    "Correzione interlinea", // Italian
    "", // Dutch
    "", // Portuguese
    "Espacement des lignes", // French
    "", // Norwegian
    "Riviv‰lin hienos‰‰tˆ", // Finnish
    "", // Polish
    "Ajuste distancia entre lÌneas", // Spanish
    "", // Greek
    "RadavstÂnd", // Swedish
    "", // Romanian
    "", // Hungarian
    "Ajustament distancia entre lÌnies", // Catalenian
    "¿–··‚ﬁÔ›ÿ’ ‹’÷‘„ ·‚‡ﬁ⁄–‹ÿ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Workaround for some French chns", // English
    "Workaround f¸r franzˆsische Kan‰le", // German
    "", // Slovenian
    "Correzione per canali francesi", // Italian
    "", // Dutch
    "", // Portuguese
    "Contournement chaÓnes franÁaises", // French
    "", // Norwegian
    "Ranskalainen silm‰nk‰‰ntˆtemppu", // Finnish
    "", // Polish
    "CorrecciÛn para canales franceses", // Spanish
    "", // Greek
    "Fix fˆr vissa franska kanaler", // Swedish
    "", // Romanian
    "", // Hungarian
    "CorrecciÛ per a canals francesos", // Catalenian
    "¡ﬂ’Ê. ›–·‚‡ﬁŸ⁄ÿ ‘€Ô ‰‡–›Ê. ⁄–›–€ﬁ“", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Idioma", // Spanish
    "", // Greek
    "SprÂk", // Swedish
    "", // Romanian
    "", // Hungarian
    "Idioma", // Catalenian
    "œ◊Î⁄", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Hearing Impaired", // English
    "Gehˆrlose", // German
    "", // Slovenian
    "Non udente", // Italian
    "", // Dutch
    "", // Portuguese
    "Malentendant", // French
    "", // Norwegian
    "kuulorajoitteisille", // Finnish
    "", // Polish
    "No oyente", // Spanish
    "", // Greek
    "Nedsatt hˆrsel", // Swedish
    "", // Romanian
    "", // Hungarian
    "Discapacitats auditius", // Catalenian
    "‘€Ô ·€–—ﬁ·€ÎË–ÈÿÂ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Font Size (pixel)", // English
    "", // German
    "", // Slovenian
    "", // Italian
    "", // Dutch
    "", // Portuguese
    "Taille Police (pixel)", // French
    "", // Norwegian
    "Kirjasintyypin koko (px)", // Finnish
    "", // Polish
    "", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Text Color", // English
    "Textfarbe", // German
    "", // Slovenian
    "Colore testo", // Italian
    "", // Dutch
    "", // Portuguese
    "Couleur du text", // French
    "", // Norwegian
    "Kirjasimen v‰ri", // Finnish
    "", // Polish
    "Color del texto", // Spanish
    "", // Greek
    "F‰rg pÂ text", // Swedish
    "", // Romanian
    "", // Hungarian
    "Color del text", // Catalenian
    "∆“’‚ ‚’⁄·‚–", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Background Color", // English
    "Hintergrundfarbe", // German
    "", // Slovenian
     "Colore sfondo", // Italian
    "", // Dutch
    "", // Portuguese
    "Couleur du fond", // French
    "", // Norwegian
    "Taustav‰ri", // Finnish
    "", // Polish
    "Color de fondo", // Spanish
    "", // Greek
    "F‰rg pÂ bakgrund", // Swedish
    "", // Romanian
    "", // Hungarian
    "Color de fons", // Catalenian
    "∆“’‚ ‰ﬁ›–", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Negro", // Spanish
    "", // Greek
    "svart", // Swedish
    "", // Romanian
    "", // Hungarian
    "Negre", // Catalenian
    "ÁÒ‡›ÎŸ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Blanco", // Spanish
    "", // Greek
    "vit", // Swedish
    "", // Romanian
    "", // Hungarian
    "Blanc", // Catalenian
    "—’€ÎŸ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Rojo", // Spanish
    "", // Greek
    "rˆd", // Swedish
    "", // Romanian
    "", // Hungarian
    "Vermell", // Catalenian
    "⁄‡–·›ÎŸ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Green", // English
    "Gr¸n", // German
    "", // Slovenian
    "Verde", // Italian
    "", // Dutch
    "", // Portuguese
    "Vert", // French
    "", // Norwegian
    "vihre‰", // Finnish
    "", // Polish
    "Verde", // Spanish
    "", // Greek
    "grˆn", // Swedish
    "", // Romanian
    "", // Hungarian
    "Verd", // Catalenian
    "◊’€Ò›ÎŸ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Amarillo", // Spanish
    "", // Greek
    "gul", // Swedish
    "", // Romanian
    "", // Hungarian
    "Groc", // Catalenian
    "÷Ò€‚ÎŸ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Magenta", // Spanish
    "", // Greek
    "magenta", // Swedish
    "", // Romanian
    "", // Hungarian
    "Magenta", // Catalenian
    "ﬂ„‡ﬂ„‡›ÎŸ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Azul", // Spanish
    "", // Greek
    "blÂ", // Swedish
    "", // Romanian
    "", // Hungarian
    "Blau", // Catalenian
    "·ÿ›ÿŸ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Cyan", // English
    "T¸rkis", // German
    "", // Slovenian
    "Ciano", // Italian
    "", // Dutch
    "", // Portuguese
    "Cyan", // French
    "", // Norwegian
    "syaani", // Finnish
    "", // Polish
    "Cian", // Spanish
    "", // Greek
    "cyan", // Swedish
    "", // Romanian
    "", // Hungarian
    "Cian", // Catalenian
    "”ﬁ€„—ﬁŸ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
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
    "Gris", // Spanish
    "", // Greek
    "grÂ", // Swedish
    "", // Romanian
    "", // Hungarian
    "Gris", // Catalenian
    "·’‡ÎŸ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Transparent", // English
    "Transparent", // German
    "", // Slovenian
    "Trasparente", // Italian
    "", // Dutch
    "", // Portuguese
    "Transparent", // French
    "", // Norwegian
    "l‰pin‰kyv‰", // Finnish
    "", // Polish
    "Transparente", // Spanish
    "", // Greek
    "transparent", // Swedish
    "", // Romanian
    "", // Hungarian
    "Transparent", // Catalenian
    "ﬂ‡ﬁ◊‡–Á›ÎŸ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Custom Color", // English
    "Benutzerdefinierte Farbe", // German
    "", // Slovenian
    "Colore personalizzato", // Italian
    "", // Dutch
    "", // Portuguese
    "Couleur personnalisÈe", // French
    "", // Norwegian
    "Muokattava v‰ri", // Finnish
    "", // Polish
    "Color personalizado", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Color personalitzat", // Catalenian
    "Ω–·‚‡–ÿ“–’‹ÎŸ Ê“’‚", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Custom", // English
    "Benutzerdefiniert", // German
    "", // Slovenian
    "Personalizzato", // Italian
    "", // Dutch
    "", // Portuguese
    "PersonnalisÈ", // French
    "", // Norwegian
    "muokattava", // Finnish
    "", // Polish
    "Personalizado", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Personalitzat", // Catalenian
    "›–·‚‡–ÿ“–’‹ÎŸ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Red Value", // English
    "Roter Wert", // German
    "", // Slovenian
    "Valore rosso", // Italian
    "", // Dutch
    "", // Portuguese
    "Valeur Rouge", // French
    "", // Norwegian
    "Punainen", // Finnish
    "", // Polish
    "Valor rojo", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Valor vermell", // Catalenian
    "√‡ﬁ“’›Ï ⁄‡–·›ﬁ”ﬁ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Green Value", // English
    "Gr¸ner Wert", // German
    "", // Slovenian
    "Valore verde", // Italian
    "", // Dutch
    "", // Portuguese
    "Valeur Vert", // French
    "", // Norwegian
    "Vihre‰", // Finnish
    "", // Polish
    "Valor verde", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Valor vert", // Catalenian
    "√‡ﬁ“’›Ï ◊’€Ò›ﬁ”ﬁ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Blue Value", // English
    "Blauer Wert", // German
    "", // Slovenian
    "Valore azzurro", // Italian
    "", // Dutch
    "", // Portuguese
    "Valeur Bleu", // French
    "", // Norwegian
    "Sininen", // Finnish
    "", // Polish
    "Valor azul", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Valor blau", // Catalenian
    "√‡ﬁ“’›Ï ·ÿ›’”ﬁ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Transparency Value", // English
    "Transparenter Wert", // German
    "", // Slovenian
    "Valore trasparenza", // Italian
    "", // Dutch
    "", // Portuguese
    "Valeur de transparence", // French
    "", // Norwegian
    "L‰pin‰kyvyys", // Finnish
    "", // Polish
    "Valor transparencia", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Valor transparËncia", // Catalenian
    "¡‚’ﬂ’›Ï ﬂ‡ﬁ◊‡–Á›ﬁ·‚ÿ", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "DVB Source Selection", // English
    "DVB Quelle Auswahl", // German
    "", // Slovenian
    "Selezione sorgente DVB", // Italian
    "", // Dutch
    "", // Portuguese
    "SÈlection Source DVB", // French
    "", // Norwegian
    "Tekstitysl‰hteen valinta", // Finnish
    "", // Polish
    "SelecciÛn fuente DVB", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "SelecciÛ font DVB", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "All", // English
    "Alle", // German
    "", // Slovenian
    "Tutte", // Italian
    "", // Dutch
    "", // Portuguese
    "Tous", // French
    "", // Norwegian
    "kaikki", // Finnish
    "", // Polish
    "Todas", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Totes", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Only DVB-S", // English
    "Nur DVB-S", // German
    "", // Slovenian
    "Solo DVB-S", // Italian
    "", // Dutch
    "", // Portuguese
    "Seulement DVB-S", // French
    "", // Norwegian
    "vain DVB-S", // Finnish
    "", // Polish
    "SÛlo DVB-S", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "NomÈs DVB-S", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Only DVB-T", // English
    "Nur DVB-T", // German
    "", // Slovenian
    "Solo DVB-T", // Italian
    "", // Dutch
    "", // Portuguese
    "Seulement DVB-T", // French
    "", // Norwegian
    "vain DVB-T", // Finnish
    "", // Polish
    "SÛlo DVB-T", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "NomÈs DVB-T", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Only DVB-C", // English
    "Nur DVB-C", // German
    "", // Slovenian
    "Solo DVB-C", // Italian
    "", // Dutch
    "", // Portuguese
    "Seulement DVB-C", // French
    "", // Norwegian
    "vain DVB-C", // Finnish
    "", // Polish
    "SÛlo DVB-C", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "NomÈs DVB-C", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Page Mode", // English
    "Seitenmodus", // German
    "", // Slovenian
    "Modo pagina", // Italian
    "", // Dutch
    "", // Portuguese
    "Mode page", // French
    "", // Norwegian
    "sivun valinta", // Finnish
    "", // Polish
    "Modo p·gina", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Modalitat p‡gina", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Page Selection", // English
    "Seitenauswahl", // German
    "", // Slovenian
    "Selezione pagina", // Italian
    "", // Dutch
    "", // Portuguese
    "Page de sÈlection", // French
    "", // Norwegian
    "Tekstityssivun valinta", // Finnish
    "", // Polish
    "SelecciÛn p·gina subtÌtulos", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "SelecciÛ p‡gina subtÌtols", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Subtitle Mode", // English
    "Untertitelmodus", // German
    "", // Slovenian
    "Modo sottotitoli", // Italian
    "", // Dutch
    "", // Portuguese
    "Mode de sous-titres TXT", // French
    "", // Norwegian
    "Tekstitystapa", // Finnish
    "", // Polish
    "Modo subtÌtulos", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Modalitat subtÌtols", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Manual Subtitle Page", // English
    "Manuelle Untertitelseite", // German
    "", // Slovenian
    "Pagina manuale sottotitoli", // Italian
    "", // Dutch
    "", // Portuguese
    "Page des sous-titres", // French
    "", // Norwegian
    "Asetettu tekstityssivu", // Finnish
    "", // Polish
    "P·gina manual subtÌtulos", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "P‡gina manual subtÌtols", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Subtitle Page Settings", // English
    "Untertitelseiten Einstellung", // German
    "", // Slovenian
    "Settaggi pagina sottotitoli", // Italian
    "", // Dutch
    "", // Portuguese
    "Conf. Page sous-titres TXT", // French
    "", // Norwegian
    "Tekstityssivun asetukset", // Finnish
    "", // Polish
    "Opciones p·gina subtÌtulos", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Opcions p‡gina subtÌtols", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Auto", // English
    "Automatisch", // German
    "", // Slovenian
    "Automatico", // Italian
    "", // Dutch
    "", // Portuguese
    "Auto", // French
    "", // Norwegian
    "automaattinen", // Finnish
    "", // Polish
    "Autom·tico", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Autom‡tic", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Manual", // English
    "Manuell", // German
    "", // Slovenian
    "Manuale", // Italian
    "", // Dutch
    "", // Portuguese
    "Manuel", // French
    "", // Norwegian
    "manuaalinen", // Finnish
    "", // Polish
    "Manual", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Manual", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Disabled", // English
    "Deaktiviert", // German
    "", // Slovenian
    "Disattivati", // Italian
    "", // Dutch
    "", // Portuguese
    "DÈsactivÈ", // French
    "", // Norwegian
    "pois", // Finnish
    "", // Polish
    "Desactivados", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Desactivats", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Live Delay", // English
    "Live Verzˆgerung", // German
    "", // Slovenian
    "Ritardo in diretta", // Italian
    "", // Dutch
    "", // Portuguese
    "", // French
    "", // Norwegian
    "Viive Live-l‰hetyksess‰", // Finnish
    "", // Polish
    "Retardo en directo", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Retard en directe", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Replay Delay", // English
    "Wiedergabe Verzˆgerung", // German
    "", // Slovenian
    "Ritardo in riproduzione", // Italian
    "", // Dutch
    "", // Portuguese
    "", // French
    "", // Norwegian
    "Viive toistossa", // Finnish
    "", // Polish
    "Retardo en reproducciÛn", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Retard en reproducciÛ", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { NULL }
};
