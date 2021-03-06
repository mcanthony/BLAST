#ifndef __IMATSH_H
#define __IMATSH_H

#include "Matcher.h"
#include <time.h>
#include <string>

#define EXIT_GOOD 104
#define EXIT_BAD 101
#define MAXSTR 2048
#define SKIP_TARGET_IDENTITY 0

const float MAXRANDINT = RAND_MAX; // 2147483648.0f;

uint32_t index_to_track_id(vector<off_t>* track_offsets, uint32_t imatsh_id);
uint32_t index_to_track_pos(vector<off_t>* track_offsets, uint32_t track_id, uint32_t imatsh_id);
void _get_arg(int argc, const char* argv[], int* c, int* a, const char* str);
void _get_arg(int argc, const char* argv[], int* c, float* a, const char* str);

typedef struct nnresult {
  float dist;
  uint32_t query_pos;
  uint32_t track_pos;
  uint32_t media_idx; 
  string media;
} NNresult;

bool operator< (NNresult const &a, NNresult const &b);
bool operator> (NNresult const &a, NNresult const &b);

typedef std::multiset<NNresult, std::less<NNresult> > MatshupSet;
typedef MatshupSet::iterator Mit;
typedef std::pair<Mit, Mit> MitPair;

class aCollage : public Matcher {
 protected:
  SeriesOfVectors* targetFeatures;
  SeriesOfVectors* targetPowers;
  SeriesOfVectors* sourceFeatures;
  SeriesOfVectors* sourcePowers;

  std::vector<off_t> *track_offsets;
  std::vector<string> *media;

  MatshupSet *matshup_set;

  idxT shingleSize;
  int hopSize;
  int queueSize;
  int loFeature;
  int hiFeature;
  idxT featureDim;

  long loadTarget(const char* fileName, const char* powerName);
  long loadSources(const char* fileList, const char* powerList, const char* mediaList, const char* targetName);
  long countVectors(const char* filename);
  long loadVectors(SeriesOfVectors* s, const char* filename, idxT dim, off_t offset=0);
  
  void computeSeriesMeanPowers();

 public:
  aCollage(const char* targetFile, const char* sourceListFile, int shingleSize, int hopSize, int queueSize);
  aCollage(const char* targetFile, const char* powerFile, const char* sourceListFile, const char* powerListFile, 
	 const char* mediaListFile, int shingleSize, int hopSize, int queueSize=0, int loFeature=0, int hiFeature=0);
  virtual ~aCollage();
  int doMatshup();
  int doMultaCollageup(uint32_t numHits, int randomMatch=0);
  MatshupSet * getResultSet();
  static void error(const char* format, ... );

};

#endif // __IMATSH_H
