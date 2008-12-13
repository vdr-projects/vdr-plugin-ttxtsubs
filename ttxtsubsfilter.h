
// ----- cTtxtSubsFilter -----

// Filters out the pages on one teletext stream 
class cTtxtSubsFilter
{
 public:
  cTtxtSubsFilter(void);
  virtual ~cTtxtSubsFilter(void);

  void SetPages(int count, uint16_t *pages);

  // indata and outdata are pointers to a ttxt data row buffers,
  // if return value <> 0 outdata countains data to pass on
  int Filter(char *indata, char *outdata);

 private:
  void FreePages(void);
  int Find(int count, uint16_t *pages, uint16_t pageno);
  void MakeY0(char *outdata, char *indata, uint16_t newpageno);

  int mCount;          // number of pages to filter out
  uint16_t *mPages;    // array of page numbers
  int mMagPage[8];     /* index into pages for each mag number, or -1 if
			  not currently collecting pages for that mag */
};

