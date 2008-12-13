

#define TTXT_INITIAL_PAGE 1
#define TTXT_SUBTITLE_PAGE 2
#define TTXT_ADDITIONAL_INFO_PAGE 3
#define TTXT_PROGRAMME_SCHEDULE_PAGE 4
#define TTXT_SUBTITLE_HEARING_IMPAIRED_PAGE 5

struct ttxtpageinfo {
  char lang[3];
  uint8_t type;
  uint8_t mag;
  uint8_t page;
};

struct ttxtpidinfo {
    uint16_t pid;
    int pagecount;
    struct ttxtpageinfo *i;
};

// XXX should be an object instead and use c++ vectors etc
struct ttxtinfo {
  int pidcount;
  struct ttxtpidinfo *p;
};

/*
 * find the ttxt_info in the PMT via the PAT, try first with the SID
 * and if that fails with the VPID
 * return <> 0 on error;
 */
int GetTtxtInfo(int card_no, int channel, uint16_t sid, uint16_t vpid, struct ttxtinfo *info);

void FreeTtxtInfoData(struct ttxtinfo *info);
void DupTtxtInfo(struct ttxtinfo *in, struct ttxtinfo *out);

struct ttxtpidinfo *FindSubs(struct ttxtinfo *info, int *pid, int *pageno);

void ClearSICache(void);
