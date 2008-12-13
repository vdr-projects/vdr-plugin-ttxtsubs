
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
#if defined(APIVERSNUM) && APIVERSNUM < 10503
    return laG0_nat_opts[no][laG0_nat_replace_map[c]];
#else
    if (cCharSetConv::SystemCharacterTable())
       return laG0_nat_opts[no][laG0_nat_replace_map[c]];
    else
       return laG0_nat_opts16[no][laG0_nat_replace_map[c]];
#endif
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


#if 0
void
show_page(struct ttxt_page *p)
{
  static cOsdBase *o = NULL;
  static tWindowHandle w1 = 0;
  static tWindowHandle w2 = 0;
  static int open1 = 0, open2 = 0;
  char *s1 = NULL, *s2 = NULL;
  int i, j;

  for(i = 1; i < TTXT_ROWS; i++) {
    char *s = NULL;
    if(p->data[i][0]) {
      int do_incr = 0;
      dprint("Checking row %d\n", i);
      for(j = 0; j < 40; j++) {
	p->data[i][j] = p->data[i][j] & 0x7f;
	switch(p->data[i][j]) {
	case 0x0d: // double height
	case 0x0f: // double size
	  do_incr = 1;
	  break;
	case 0x0a: // box end
	  p->data[i][j] = '\0';
	  break;
	case 0x0b: // box begin
	  s = (char *) &(p->data[i][j+1]);
	  dprint("Using row %d\n", i);
	  break;
	case '[':
	  p->data[i][j] = 'Ä';
	  break;
	case ']':
	  p->data[i][j] = 'Å';
	  break;
	case '\\':
	  p->data[i][j] = 'Ö';
	  break;
	case '{':
	  p->data[i][j] = 'ä';
	  break;
	case '}':
	  p->data[i][j] = 'å';
	  break;
	case '|':
	  p->data[i][j] = 'ö';
	  break;
	}
      }
      if(do_incr)
	i++;
      dprint("\n");
    }

    if(s)
      if(!s1)
	s1 = s;
      else if(!s2)
	s2 = s;
  }

  if(s1 && !s2) {
    s2 = s1;
    s1 = NULL;
  }

  if(s1)
    dprint("S1: %s\n", s1);
  else
    dprint("S1: <none>\n");
  if(s2)
    dprint("S2: %s\n", s2);
  else
    dprint("S2: <none>\n");

  if(o != NULL) {
    if(open1) {
      o->Clear(w1);
      open1 = 0;
    }
    if(open2) {
      o->Clear(w2);
      open2 = 0;
    }

    o->Clear();
    delete o;
    o = NULL;
  }

  if(!s1 && !s2)
    return;

  o = cOsd::OpenRaw(125, 450);
  
  if(o == NULL)
    return;

  o->SetFont(fontOsd);

  if(s1) {
    int w = o->Width((char *) s1);
    dprint("w1: %d\n", w);
    w = 450;
    int h = 36;
    w1 = o->Create(0, 0, w, h, 2);
#if 0
    o->AddColor(clrBackground, w1);
    o->AddColor(clrWhite, w1);
#endif
    o->Fill(0, 0, w, h, clrBackground, w1);
    o->Text(15, 4, (char *) s1, clrWhite, clrBackground, w1);
    open1 = 1;
  }

  if(s2) {
    int w = o->Width((char *) s2);
    dprint("w2: %d\n", w);
    w = 452;
    int h = 36;
    w2 = o->Create(0, 45, w, h, 2);
#if 0
    o->AddColor(clrBackground, w2);
    o->AddColor(clrWhite, w2);
#endif
    o->Fill(0, 45, w, 45 + h, clrBackground, w2);
    o->Text(15, 45 + 4, (char *) s2, clrWhite, clrBackground, w2);
    open2 = 1;
  }

  o->Flush();
}
#endif

#if 0
void
ttxt_packet_in(int data_unit_id, int mag, int pack, uint8_t *data)
{
  static int valid = 0;
  static struct ttxt_page p;
  int i;
  
  if(pack == 0) {
    if(valid)
      if(p.mag == TTXT_MAG && p.no == TTXT_PAGE)
	if(p.subtitle)
	  //print_page(&p);
	  /* if(p.mag == 1) */{
	  show_page(&p);
	  print_page(&p);
	}

    memset(&p, 0, sizeof(p));
    p.data_unit_id = data_unit_id;
    p.mag = mag;
    p.no = unham(data[0], data[1]);
    
    if(data[3] & 0x80) { // Erase Page
      memset(p.data, 0, sizeof(p.data));
      p.erasepage = 1;
    }
    if(data[5] & 0x20) // Newsflash
      p.newsflash = 1;
    if(data[5] & 0x80) // Subtitle
      p.subtitle = 1;
    if(data[6] & 0x02) // Suppress Header
      p.suppress_header = 1;
    //if(data[6] & 0x08) // Update Indicator
    //if(data[6] & 0x20) // Interrupted Sequence
    if(data[6] & 0x80) // Inhibit Display
      p.inhibit_display = 1;
    //if(data[7] & 0x02) // Magazine Serial
    
    p.national_charset = 
      ((data[7] & 0x80) >> 7) + ((data[7] & 0x20) >> 4) + ((data[7] & 0x08) >> 1);

    valid = 1;

    for(i = 0; i < 32; i++)
      p.data[0][i] = data[i+8];
  } else if(pack < 26) {
    for(i = 0; i < 42; i++)
      p.data[pack][i] = data[i];
  } else {
    if((valid && p.mag == TTXT_MAG && p.no == TTXT_PAGE) ||
       pack == 28)
	dprint("mag: %d, pack: %d, page: %02x\n", p.mag, pack, p.no);
  }
}
#endif

#if 0
void
ttxt_pes_data_in(struct ttxt_pes_data_field *t, int len)
{
  int i;

  if(len < 1)
    return;

  if(t->data_identifier < 0x10 || t->data_identifier > 0x1F) {
    dprint("Unknown data identifier: %02x\n", t->data_identifier);
    return;
  }

  for(i = 0; (uint8_t *) (&(t->d[i+1])) < (((uint8_t *) t) + len); i++) {
    struct ttxt_data_field *d = &(t->d[i]);
    int i;
    uint8_t *p;
    int uh;
    int mag;
    int pack;

    //    if(d->data_unit_id != ttxt_subtitle)
    //      continue;

    p = (uint8_t *) &(d->mag_addr_ham);
    for(i = 0; i < 42; i++) {
      p[i] = invtab[p[i]];
    }

    uh = unham(d->mag_addr_ham[0], d->mag_addr_ham[1]);
    mag = uh & 0x7;
    pack = (uh >> 3) & 0x1f;

    // dump_hex("line", (uint8_t *) d, 20);
    // dprint("id: %d, line: %d, mag %d, pack: %d\n", d->data_unit_id, TTXT_LINE_NO(*d), mag, pack);

    ttxt_packet_in(d->data_unit_id, mag, pack, d->data);
  }
}


void
set_filt(int fd,uint16_t tt_pid, dmx_pes_type_t t)
{
	struct dmx_pes_filter_params pesFilterParams;

	memset(&pesFilterParams, 0, sizeof(pesFilterParams));

	pesFilterParams.pid     = tt_pid;
	pesFilterParams.input   = DMX_IN_FRONTEND;
	pesFilterParams.output  = DMX_OUT_TAP;
        pesFilterParams.pes_type = t;
	pesFilterParams.flags   = DMX_IMMEDIATE_START;

	if (ioctl(fd, DMX_SET_PES_FILTER, &pesFilterParams) < 0)  
		perror("DMX SET PES FILTER:");

	// XXX set buffer size too?
}
#endif

#if 0
#define BUFSIZE 2000

// external entry point
void ttxt_decode(int fd, int tpid, int *doRun)
{
  uint8_t buf[BUFSIZE];
  int n;
  int hunt = 1;

  set_filt(fd, tpid, DMX_PES_OTHER); 

  while (*doRun) {
  restart:
    if(hunt) {
      int i;

      dprint("hunting...\n");
      n = read(fd, buf, 199);
      if(n < 0) {
	perror("ttxt_decode:read: ");
	goto restart;
      }

      for(i = 0; i < (n - 3); i++) {
	if(buf[i] == 0 && buf[i+1] == 0 && buf[i+2] == 1 && buf[i+3] == 0xbd) {
	  struct ttxt_pes_header *p = (struct ttxt_pes_header *) (buf + i);
	  size_t skip = ntohs(p->len) + 6 - (n - i);

	  if(skip > 0) {
	    read(fd, buf, skip < sizeof(buf) ? skip : sizeof(buf));
	    if(n < 0) {
	      perror("ttxt_decode:read: ");
	      goto restart;
	    }
	    hunt = 0;
	    break;
	  }
	}
      }
    } else {
      struct ttxt_pes_header *p = (struct ttxt_pes_header *) buf;

      n = read(fd, buf, 6);
      if(n < 0) {
	perror("ttxt_decode:read: ");
	goto restart;
      }

      if(p->start[0] != 0 || p->start[1] != 0 || p->start[2] != 1 ||
	 p->stream_id != 0xbd || // private_stream_1
	 ntohs(p->len) > (sizeof(buf) - 6)) { 
	dprint("Lost PES packet sync, hunting...\n");
	hunt = 1;
	continue;
      }

      n = read(fd, buf + 6, ntohs(p->len));
      if(n < 0) {
	perror("ttxt_decode:read: ");
	goto restart;
      }

      // dprint("len: %d\n", n);

      if(n != ntohs(p->len) ||
	 p->header_len != 0x24) { // header should allways be 0x24 long (EN 300 472)
	dprint("Error reading PES packet, hunting...\n");
	hunt = 1;
	continue;
      }

      ttxt_pes_data_in((struct ttxt_pes_data_field *) (buf + 45), n + 6 - 45);
    }

    //    fwrite(buf,1,n,stdout);
  }

  dprint("ttxt_decode DONE!\n");
}
#endif


#if 0
int
main(int argc, char **argv)
{
  int fd;
  int pid;
  uint8_t buf[BUFSIZE];
  int n;
  int hunt = 1;
  
  if (argc!=2) {
    fprintf(stderr,"USAGE: %s PID\n", argv[0]);
    return -1;
  }

  pid = atoi(argv[1]);

  if((fd = open("/dev/dvb/adapter0/demux0", O_RDWR)) < 0){
    perror("Error opening demux device: ");
    return -1;
  }

  set_filt(fd, pid, DMX_PES_OTHER); 

  for (;;) {
    if(hunt) {
      int i;

      dprint("hunting...\n");
      n = read(fd, buf, 199);

      for(i = 0; i < (n - 3); i++) {
	if(buf[i] == 0 && buf[i+1] == 0 && buf[i+2] == 1 && buf[i+3] == 0xbd) {
	  struct ttxt_pes_header *p = (struct ttxt_pes_header *) (buf + i);
	  int skip = ntohs(p->len) + 6 - (n - i);

	  if(skip > 0) {
	    read(fd, buf, skip < sizeof(buf) ? skip : sizeof(buf));
	    hunt = 0;
	    break;
	  }
	}
      }
    } else {
      struct ttxt_pes_header *p = (struct ttxt_pes_header *) buf;

      n = read(fd, buf, 6);

      if(p->start[0] != 0 || p->start[1] != 0 || p->start[2] != 1 ||
	 p->stream_id != 0xbd || // private_stream_1
	 ntohs(p->len) > (sizeof(buf) - 6)) { 
	dprint("Lost PES packet sync, hunting...\n");
	hunt = 1;
	continue;
      }

      n = read(fd, buf + 6, ntohs(p->len));

      // dprint("len: %d\n", n);

      if(n != ntohs(p->len) ||
	 p->header_len != 0x24) { // header should allways be 0x24 long (EN 300 472)
	dprint("Error reading PES packet, hunting...\n");
	hunt = 1;
	continue;
      }

      ttxt_pes_data_in((struct ttxt_pes_data_field *) (buf + 45), n + 6 - 45);
    }

    //    fwrite(buf,1,n,stdout);
  }

  return 0;
}
#endif
