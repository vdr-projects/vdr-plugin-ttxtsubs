
#ifndef __TS_TELETEXT_H
#define __TS_TELETEXT_H


#define PACK  __attribute__ ((__packed__))

struct ttxt_pes_header {
  uint8_t start[3];
  uint8_t stream_id  PACK;
  uint16_t len PACK;

  // private_stream_1 packets (and others)
  uint8_t flags[2] PACK;
  uint8_t header_len PACK;
};


enum {
  ttxt_non_subtitle = 2,
  ttxt_subtitle = 3,
  ttxt_stuff = 0xff
};

#define TTXT_LINE_NO(x) ((x).par_loff & 0x1f)
struct ttxt_data_field {
  uint8_t data_unit_id PACK;
  uint8_t data_unit_length PACK;
  uint8_t par_loff PACK;
  uint8_t framing_code PACK;
  uint8_t mag_addr_ham[2] PACK;
  uint8_t data[40] PACK;
};


struct ttxt_pes_data_field {
  uint8_t data_identifier PACK;
  struct ttxt_data_field d[1] PACK;
};


// Really 25 + row 0
#define TTXT_DISPLAYABLE_ROWS 26

enum pageflags {
  erasepage = 0x01,
  newsflash = 0x02,
  subtitle = 0x04,
  suppress_header = 0x08,
  inhibit_display = 0x10 // rows 1 - 24 are not to be displayed
};

struct ttxt_page {
  uint8_t mag;
  uint8_t no;
  uint8_t flags;
  uint8_t national_charset;
  uint8_t data[TTXT_DISPLAYABLE_ROWS][40];
};

// FROM vbidecode
// unham 2 bytes into 1, report 2 bit errors but ignore them
unsigned char unham(unsigned char a,unsigned char b);

#define UNHAM_INV(a, b) unham(invtab[a], invtab[b])

// ham 8/4 the byte in into two bytes pointed to by out
void ham8_4byte(uint8_t in, uint8_t *out);
// ham 8/4 the nibble in into the byte pointed to by out
uint8_t ham8_4nibble(uint8_t in);

// odd parity status for last 7 bits of byte 
int parity(uint8_t x);

/*
 * Map Latin G0 teletext characters into a ISO-8859-1 approximation.
 * Trying to use similar looking or similar meaning characters.
 * Gtriplet - 4 bits = triplet-1 bits 14-11 (14-8) of a X/28 or M/29 packet, if unknown - use 0
 * natopts - 3 bits = national_options field of a Y/0 packet (or triplet-1 bits 10-8 as above?)
 * inchar - 7 bits = characted to remap
 * Also strips parity
 */
uint8_t ttxt_laG0_la1_char(int Gtriplet, int natopts, uint8_t inchar);

/*
 * Map Latin G2 teletext characters into a ISO-8859-1 approximation.
 * Trying to use similar looking or similar meaning characters.
 */
uint8_t ttxt_laG2_la1_char(uint8_t inchar);



// debug stuff
void print_page(struct ttxt_page *p);
void print_line(char *p);
void dump_hex(char *msg, const uint8_t *p, int len);

// invert tab for last 42 bytes in packets
extern unsigned char invtab[256];

#endif
