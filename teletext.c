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

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <ctype.h>
#include <string.h>

//#include <linux/dvb/dmx.h>
//#include <vdr/osd.h>
#include <vdr/config.h>

#include "teletext.h"
#include "teletext-tables.h"
#include "teletext-chars.h"
#include "utils.h"


// FROM vbidecode
// unham 2 bytes into 1, report 2 bit errors but ignore them
unsigned char unham(unsigned char a,unsigned char b)
{
  unsigned char c1,c2;
  
  c1=unhamtab[a];
  c2=unhamtab[b];
//  if ((c1|c2)&0x40) 
//      dprint("bad ham!");
  return (c2<<4)|(0x0f&c1);
}


// ham 8/4 the byte in into two bytes pointed to by out
// should make a table instead
void ham8_4byte(uint8_t in, uint8_t *out)
{
  out[0] = ham8_4nibble(in & 0xF);
  out[1] = ham8_4nibble(in >> 4);

  if (1) { // XXX debug
    int a;
    if(unham(out[0], out[1]) != in) {
      dprint("ham8_4: 1 - result not correct %02x -> %02x %02x!\n", in, out[0], out[1]);
    }
    a = unhamtab[out[0]];
    a ^= 0x80;
    if(a & 0xf0 || (a != (in & 0xF))) {
      dprint("ham8_4: 2 - result not correct %02x -> %02x %02x, %02x!\n", in, out[0], out[1], a);
    }
    a = unhamtab[out[1]];
    a ^= 0x80;
    if(a & 0xf0 || (a != (in >> 4))) {
      dprint("ham8_4: 3 - result not correct %02x -> %02x %02x, %02x!\n", in, out[0], out[1], a);
    }
  }
}

// should be a table instead
int parity(uint8_t x) {
  int res = 1;
  int count = x & 0xf0 ? 8 : 4;

  while(count--) {
    if(x & 0x01)
      res = !res;
    x >>= 1;
  }

  return res;
}

// ham 8/4 the nibble in into the byte pointed to by out
// should make a table instead
// D4 P4 D3 P3 D2 P2 D1 P1
// P4 = all the rest
uint8_t ham8_4nibble(uint8_t in)
{
  uint8_t o = 0;

  // insert the data bits
  o |= (in << 4) & 0x80;
  o |= (in << 3) & 0x20;
  o |= (in << 2) & 0x08;
  o |= (in << 1) & 0x02;

  if(parity(in & 0x0d)) // P1 = 1 . D1 . D3 . D4
    o |= 0x01;
  if(parity(in & 0x0b)) // P2 = 1 . D1 . D2 . D4
    o |= 0x04;
  if(parity(in & 0x07)) // P3 = 1 . D1 . D2 . D3
    o |= 0x10;
  if(parity(o & 0xbf)) // P4 = 1 . P1 . D1 . ... . D3 . D4
    o |= 0x40;

  return o;
}


/*
 * Map Latin G0 teletext characters into a ISO-8859-1 approximation.
 * Trying to use similar looking or similar meaning characters.
 * Gtriplet - 4 bits = triplet-1 bits 14-11 (14-8) of a X/28 or M/29 packet, if unknown - use 0
 * natopts - 3 bits = national_options field of a Y/0 packet (or triplet-1 bits 10-8 as above?)
 * inchar - 7 bits = characted to remap
 * Also strips parity
 */

uint16_t ttxt_laG0_la1_char(int Gtriplet, int natopts, uint8_t inchar)
{
  int no = laG0_nat_opts_lookup[Gtriplet & 0xf][natopts & 0x7];
  uint8_t c = inchar & 0x7f;

  //dprint("\n%x/%c/%x/%x\n", c, c, laG0_nat_replace_map[c], laG0_nat_opts[no][laG0_nat_replace_map[c]]);

  if(!laG0_nat_replace_map[c])
    return c;
  else
    if (cCharSetConv::SystemCharacterTable())
       return laG0_nat_opts[no][laG0_nat_replace_map[c]];
    else
       return laG0_nat_opts16[no][laG0_nat_replace_map[c]];
}

/*
 * Map Latin G2 teletext characters into a ISO-8859-1 approximation.
 * Trying to use similar looking or similar meaning characters.
 */
uint16_t ttxt_laG2_la1_char(uint8_t inchar)
{
  return laG2_la1_table[inchar & 0x7f];
}

void dump_hex(char *msg, const uint8_t *p, int len)
{
  int i;
  printf("%s:", msg);
  for(i = 0; i < len; i++) {
    printf(" %02x", p[i]);
  }
  printf("\n");
}


void
print_code(uint8_t c, int natopts)
{
  if(c >= 0x20) {
    dprint("%c", ttxt_laG0_la1_char(0, natopts, c));
  } else
  switch(c) {
  case 0x0:
  case 0x1:
  case 0x2:
  case 0x3:
  case 0x4:
  case 0x5:
  case 0x6:
  case 0x7:
    dprint("<fc%d>", c);
    break;
  case 0x08:
    dprint("<flash>");
    break;
  case 0x09:
    dprint("<Steady>");
    break;
  case 0x0a:
    dprint("]");
    break;
  case 0x0b:
    dprint("[");
    break;
  case 0x0c:
    dprint("<normal size>");
    break;
  case 0x0d:
    dprint("<dh>");
    break;
  case 0x0e:
    dprint("<dbl width>");
    break;
  case 0x0f:
    dprint("<dbl size>");
    break;
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
    dprint("<mosc%d>", c-0x10);
    break;
  case 0x18:
    dprint("<conceal>");
    break;
  case 0x19:
    dprint("<cont mosaic>");
    break;
  case 0x1a:
    dprint("<sep mosaic>");
    break;
  case 0x1b:
    dprint("<charset switch>");
    break;
  case 0x1c:
    dprint("<black bg>");
    break;
  case 0x1d:
    dprint("<new bg>");
    break;
  case 0x1e:
    dprint("<hold mosaic>");
    break;
  case 0x1f:
    dprint("<rele mosaic>");
    break;
  default:
    dprint("<U%02x>", c);
  }
}

void
print_page(struct ttxt_page *p)
{
  int r, c, e;

  dprint("\n\n%03x %s%s%s%s%snatchars: %d\n",
	 (p->mag << 8) + p->no,
	 p->flags & erasepage ? "erasepage, " : "",
	 p->flags & newsflash ? "Newsflash, " : "",
	 p->flags & subtitle ? "Subtitle, " : "",
	 p->flags & suppress_header ? "SuppressHeader, " : "",
	 p->flags & inhibit_display ? "InihinitDisplay, " : "",
	 p->national_charset);

  for(r = 0; r < 26; r++) {
    int do_display = 0;

    for(e = 39; e > 0; e--) {
      if(p->data[r][e]) {
	do_display = 1;
	break;
      }
    }

    if(do_display) {
      dprint("%02d ", r);
      for(c = 0; c <= e; c++) {
	char z = p->data[r][c] & 0x7f;
	print_code(z, p->national_charset);
	//dprint("%c", isprint(z) ? z : z == 0 ? '\'' : '.');
      }
      dprint("\n");
    }
  }
}


void print_line(char *p)
{
  int mp;
  int mag; // X in ETSI EN 300 706
  int packet; // Y
  struct ttxt_data_field *d = (struct ttxt_data_field *) p;

  mp = unham(invtab[d->mag_addr_ham[0]], invtab[d->mag_addr_ham[1]]);
  mag = mp & 0x7;
  packet = (mp >> 3) & 0x1f;

  dprint("I%d Y%02d M%d ", d->data_unit_id, packet, mag);

  // packet 0, change page no in this magazine
  if(packet == 0) {
    int no;
    uint8_t buf[8];
    
    for(int i = 0; i < 8; i++)
      buf[i] = invtab[d->data[i]];

    no = unham(buf[0], buf[1]);

    dprint("P%02x ", no);

    dprint("<");
    for(int i = 8; i < 40; i++)
      print_code(invtab[d->data[i]] & 0x7f, 0);
    dprint(">");

    if(buf[3] & 0x80) dprint(", erase page");
    if(buf[5] & 0x20) dprint(", newsflash");
    if(buf[5] & 0x80) dprint(", subtitle");
    if(buf[6] & 0x02) dprint(", suppress header");
    if(buf[6] & 0x08) dprint(", update indicator");
    if(buf[6] & 0x20) dprint(", interrupted sequence");
    if(buf[6] & 0x80) dprint(", inhibit display");
    if(buf[7] & 0x02) dprint(", magazine serial");
    
    dprint(", charset=%d", ((buf[7] & 0x80) >> 5) + ((buf[7] & 0x20) >> 4) + ((buf[7] & 0x08) >> 3));
  } else {
    dprint("<");
    for(int i = 0; i < 40; i++)
      print_code(invtab[d->data[i]] & 0x7f, 0);
    dprint(">");
  }

  dprint("\n");
}
