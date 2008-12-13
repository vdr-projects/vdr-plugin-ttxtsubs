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
    "Sous-titrage t�l�texte", // French - St�phane Est�-Gracias 2004-03-02
    "", // Norwegian
    "Tekstitys (Teksti-TV)", // Finnish - Rolf Ahrenberg 2004-03-01
    "", // Polish
    "Subt�tulos teletexto", // Spanish
    "", // Greek
    "Textning (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "Subt�tols teletext", // Catalanian
    "���������-��������", // Russian - Vyacheslav Dikonov 2004-xx-xx
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
    "Esconde subt�tulos teletexto", // Spanish
    "", // Greek
    "G�m textning (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "Amaga subt�tols teletext", // Catalenian
    "������ ��������", // Russian
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
    "N�yt� tekstitys (Teksti-TV)", // Finnish
    "", // Polish
    "Visualiza subt�tulos teletexto", // Spanish
    "", // Greek
    "Visa textning (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "Visualitza subt�tols teletext", // Catalenian
    "���������� ��������", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Position Teletext Subtitles for 4:3/Anamorph", // English
    "Positioniere Videotext-Untertitel f�r 4:3/Anamorph", // German
    "", // Slovenian
    "Posizionare sottotitoli per 4:3/anamorfico", // Italian
    "", // Dutch
    "", // Portuguese
    "Position du sous-titrage pour 4:3/Anamorph", // French
    "", // Norwegian
    "4:3/Anamorfinen tekstitys (Teksti-TV)", // Finnish
    "", // Polish
    "Posiciona subt�tulos teletexto para 4:3/anam�rfico", // Spanish
    "", // Greek
    "Textning f�r 4:3/anamorf (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "Posiciona subt�tols teletext per a 4:3/anam�rfic", // Catalenian
    "�������� � ����� 4:3", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Position Teletext Subtitles for Letterbox", // English
    "Positioniere Videotext-Untertitel f�r Letterbox", // German
    "", // Slovenian
    "Posizionare sottotitoli per Letterbox", // Italian
    "", // Dutch
    "", // Portuguese
    "Position du sous-titrage pour Letterbox", // French
    "", // Norwegian
    "Letterbox tekstitys (Teksti-TV)", // Finnish
    "", // Polish
    "Posici�na subt�tulos teletexto para Letterbox", // Spanish
    "", // Greek
    "Textning f�r brevl�deformat (Text-TV)", // Swedish
    "", // Romanian
    "", // Hungarian
    "Posiciona subt�tols teletext per a Letterbox", // Catalenian
    "�������� � ����� ��������������� ������", // Russian
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
    "N�yt� tekstitykset", // Finnish
    "", // Polish
    "Visualiza subt�tulos", // Spanish
    "", // Greek
    "Visa textning", // Swedish
    "", // Romanian
    "", // Hungarian
    "Visualitza subt�tols", // Catalenian
    "���������� ��������", // Russian
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
    "Graba subt�tulos", // Spanish
    "", // Greek
    "Spela in textning", // Swedish
    "", // Romanian
    "", // Hungarian
    "Grava subt�tols", // Catalenian
    "���������� ��������", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Main Menu Alternative", // English
    "Hauptmenu Alternative", // German
    "", // Slovenian
    "Alternativa men� principale", // Italian
    "", // Dutch
    "", // Portuguese
    "Choix du menu principal", // French
    "", // Norwegian
    "P��valikon toiminnot", // Finnish
    "", // Polish
    "Alternativa del men� principal", // Spanish
    "", // Greek
    "Huvudmenyalternativ", // Swedish
    "", // Romanian
    "", // Hungarian
    "Alternativa del men� principal", // Catalenian
    "������� � ������� ����", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Display on/off", // English
    "Anzeige ein/aus", // German
    "", // Slovenian
    "Visualizza s�/no", // Italian
    "", // Dutch
    "", // Portuguese
    "Affichage", // French
    "", // Norwegian
    "aktivointi", // Finnish
    "", // Polish
    "Visualiza s�/no", // Spanish
    "", // Greek
    "Visa textning p�/av", // Swedish
    "", // Romanian
    "", // Hungarian
    "Visualitza si/no", // Catalenian
    "��������/���������", // Russian
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
    "4:3/brevl�deformat", // Swedish
    "", // Romanian
    "", // Hungarian
    "4:3/Letterbox", // Catalenian
    "4:3/������� �����", // Russian
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
    "�Posici�n horizontal del texto", // Spanish
    "", // Greek
    "Horisontell position", // Swedish
    "", // Romanian
    "", // Hungarian
    "Posici� horitzontal del text ", // Catalenian
    "���������� ������ �� �����������", // Russian
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
    "v�nster", // Swedish
    "", // Romanian
    "", // Hungarian
    "Esquerra", // Catalenian
    "�����", // Russian
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
    "keskit�", // Finnish
    "", // Polish
    "Centro", // Spanish
    "", // Greek
    "centrerad", // Swedish
    "", // Romanian
    "", // Hungarian
    "Centre", // Catalenian
    "� ������", // Russian
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
    "h�ger", // Swedish
    "", // Romanian
    "", // Hungarian
    "Dreta", // Catalenian
    "������", // Russian
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
    "Posici�n vertical del texto", // Spanish
    "", // Greek
    "Vertikal positon", // Swedish
    "", // Romanian
    "", // Hungarian
    "Posici�n vertical del text", // Catalenian
    "���������� ������ �� ���������", // Russian
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
    "4:3/Anam�rfico", // Spanish
    "", // Greek
    "4:3/anamorf", // Swedish
    "", // Romanian
    "", // Hungarian
    "4:3/Anam�rfic", // Catalenian
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
    "brevl�deformat", // Swedish
    "", // Romanian
    "", // Hungarian
    "Letterbox", // Catalenian
    "������� �����", // Russian
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
    "Pystytasauksen hienos��t�", // Finnish
    "", // Polish
    "Ajuste vertical del texto", // Spanish
    "", // Greek
    "Vertikal justering", // Swedish
    "", // Romanian
    "", // Hungarian
    "Ajustament vertical del text", // Catalenian
    "������������ ����� ������", // Russian
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
    "Riviv�lin hienos��t�", // Finnish
    "", // Polish
    "Ajuste distancia entre l�neas", // Spanish
    "", // Greek
    "Radavst�nd", // Swedish
    "", // Romanian
    "", // Hungarian
    "Ajustament distancia entre l�nies", // Catalenian
    "���������� ����� ��������", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Workaround for some French chns", // English
    "Workaround f�r franz�sische Kan�le", // German
    "", // Slovenian
    "Correzione per canali francesi", // Italian
    "", // Dutch
    "", // Portuguese
    "Contournement cha�nes fran�aises", // French
    "", // Norwegian
    "Ranskalainen silm�nk��nt�temppu", // Finnish
    "", // Polish
    "Correcci�n para canales franceses", // Spanish
    "", // Greek
    "Fix f�r vissa franska kanaler", // Swedish
    "", // Romanian
    "", // Hungarian
    "Correcci� per a canals francesos", // Catalenian
    "����. ��������� ��� �����. �������", // Russian
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
    "Spr�k", // Swedish
    "", // Romanian
    "", // Hungarian
    "Idioma", // Catalenian
    "����", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Hearing Impaired", // English
    "Geh�rlose", // German
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
    "Nedsatt h�rsel", // Swedish
    "", // Romanian
    "", // Hungarian
    "Discapacitats auditius", // Catalenian
    "��� �������������", // Russian
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
    "Kirjasimen v�ri", // Finnish
    "", // Polish
    "Color del texto", // Spanish
    "", // Greek
    "F�rg p� text", // Swedish
    "", // Romanian
    "", // Hungarian
    "Color del text", // Catalenian
    "���� ������", // Russian
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
    "Taustav�ri", // Finnish
    "", // Polish
    "Color de fondo", // Spanish
    "", // Greek
    "F�rg p� bakgrund", // Swedish
    "", // Romanian
    "", // Hungarian
    "Color de fons", // Catalenian
    "���� ����", // Russian
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
    "������", // Russian
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
    "�����", // Russian
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
    "r�d", // Swedish
    "", // Romanian
    "", // Hungarian
    "Vermell", // Catalenian
    "�������", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Green", // English
    "Gr�n", // German
    "", // Slovenian
    "Verde", // Italian
    "", // Dutch
    "", // Portuguese
    "Vert", // French
    "", // Norwegian
    "vihre�", // Finnish
    "", // Polish
    "Verde", // Spanish
    "", // Greek
    "gr�n", // Swedish
    "", // Romanian
    "", // Hungarian
    "Verd", // Catalenian
    "�������", // Russian
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
    "������", // Russian
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
    "���������", // Russian
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
    "bl�", // Swedish
    "", // Romanian
    "", // Hungarian
    "Blau", // Catalenian
    "�����", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Cyan", // English
    "T�rkis", // German
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
    "�������", // Russian
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
    "gr�", // Swedish
    "", // Romanian
    "", // Hungarian
    "Gris", // Catalenian
    "�����", // Russian
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
    "l�pin�kyv�", // Finnish
    "", // Polish
    "Transparente", // Spanish
    "", // Greek
    "transparent", // Swedish
    "", // Romanian
    "", // Hungarian
    "Transparent", // Catalenian
    "����������", // Russian
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
    "Couleur personnalis�e", // French
    "", // Norwegian
    "Muokattava v�ri", // Finnish
    "", // Polish
    "Color personalizado", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Color personalitzat", // Catalenian
    "������������� ����", // Russian
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
    "Personnalis�", // French
    "", // Norwegian
    "muokattava", // Finnish
    "", // Polish
    "Personalizado", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Personalitzat", // Catalenian
    "�������������", // Russian
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
    "������� ��������", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { "Green Value", // English
    "Gr�ner Wert", // German
    "", // Slovenian
    "Valore verde", // Italian
    "", // Dutch
    "", // Portuguese
    "Valeur Vert", // French
    "", // Norwegian
    "Vihre�", // Finnish
    "", // Polish
    "Valor verde", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Valor vert", // Catalenian
    "������� ��������", // Russian
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
    "������� ������", // Russian
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
    "L�pin�kyvyys", // Finnish
    "", // Polish
    "Valor transparencia", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Valor transpar�ncia", // Catalenian
    "������� ������������", // Russian
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
    "S�lection Source DVB", // French
    "", // Norwegian
    "Tekstitysl�hteen valinta", // Finnish
    "", // Polish
    "Selecci�n fuente DVB", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Selecci� font DVB", // Catalenian
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
    "S�lo DVB-S", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Nom�s DVB-S", // Catalenian
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
    "S�lo DVB-T", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Nom�s DVB-T", // Catalenian
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
    "S�lo DVB-C", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Nom�s DVB-C", // Catalenian
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
    "Modo p�gina", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Modalitat p�gina", // Catalenian
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
    "Page de s�lection", // French
    "", // Norwegian
    "Tekstityssivun valinta", // Finnish
    "", // Polish
    "Selecci�n p�gina subt�tulos", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Selecci� p�gina subt�tols", // Catalenian
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
    "Modo subt�tulos", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Modalitat subt�tols", // Catalenian
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
    "P�gina manual subt�tulos", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "P�gina manual subt�tols", // Catalenian
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
    "Opciones p�gina subt�tulos", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Opcions p�gina subt�tols", // Catalenian
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
    "Autom�tico", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Autom�tic", // Catalenian
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
    "D�sactiv�", // French
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
    "Live Verz�gerung", // German
    "", // Slovenian
    "Ritardo in diretta", // Italian
    "", // Dutch
    "", // Portuguese
    "", // French
    "", // Norwegian
    "Viive Live-l�hetyksess�", // Finnish
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
    "Wiedergabe Verz�gerung", // German
    "", // Slovenian
    "Ritardo in riproduzione", // Italian
    "", // Dutch
    "", // Portuguese
    "", // French
    "", // Norwegian
    "Viive toistossa", // Finnish
    "", // Polish
    "Retardo en reproducci�n", // Spanish
    "", // Greek
    "", // Swedish
    "", // Romanian
    "", // Hungarian
    "Retard en reproducci�", // Catalenian
    "", // Russian
    "", // Croatian
    "", // Eesti
    "", // Dansk
  },
  { NULL }
};
