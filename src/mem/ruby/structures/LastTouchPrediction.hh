/**
 * @file LastTouchPrediction.hh
 * @author Abhishek Bhaumick & Raghav V.
 * @brief
 * @version 0.1
 * @date 2021-04-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __MEM_RUBY_STRUCTURES_LASTTOUCHPREDICTION_HH__
#define __MEM_RUBY_STRUCTURES_LASTTOUCHPREDICTION_HH__

#include <fstream>
#include <set>
#include <vector>

#include "base/logging.hh"
#include "base/statistics.hh"
#include "mem/cache/replacement_policies/base.hh"
#include "mem/cache/replacement_policies/replaceable_entry.hh"
#include "mem/ruby/common/DataBlock.hh"
#include "mem/ruby/protocol/CacheRequestType.hh"
#include "mem/ruby/protocol/CacheResourceType.hh"
#include "mem/ruby/protocol/RubyRequest.hh"
#include "mem/ruby/slicc_interface/AbstractCacheEntry.hh"
#include "mem/ruby/slicc_interface/RubySlicc_ComponentMapping.hh"
#include "mem/ruby/structures/BankedArray.hh"
#include "mem/ruby/structures/LastTouchPrediction.hh"
#include "mem/ruby/system/CacheRecorder.hh"
#include "params/RubyCache.hh"
#include "sim/sim_object.hh"

/*
TODO

Data Structures
- signature table
  - per line. Size = number of lines
- local history table. 2-d array <number lines, linked list of traces>

*/

#define HASH_INIT 0xFFFFFFFF
#define CONFIDENCE_COUNT 2
class LoggerLT : public Logger
{
public:
  using Logger::Logger;

  LoggerLT ()
    : Logger("trace: ")
  {}

  LoggerLT (const char * prefix)
    : Logger(prefix)
  {}

  void setPrefix (const char * prefix);

  void setup(const char * prefix, int id, bool enabled);

  ~LoggerLT()
  {
    if (logFile.is_open())
      logFile.close();
  }

protected:
  bool enabled;
  std::ofstream logFile;
  void log(const Loc &loc, std::string s) override
  {
    if (enabled) {
      logFile << s << std::flush;
    }
  }
};

#define traceLog(logger, ...) \
  logger.print(::LoggerLT::Loc(__FILE__, __LINE__), __VA_ARGS__)

struct ltpTrace
{
  std::vector<Addr> PCVector;
  uint32_t hash;
  bool valid;
  int predCount;

  ltpTrace() : hash(HASH_INIT), valid(false), predCount(0) {}

  // friend std::ostream& operator<<(std::ostream & out, const ltpTrace & t);
};

std::ostream& operator<<(std::ostream & out, const ltpTrace & t);

class LTP
{

public:
  LTP() {};
  LTP(int numberOfSets, int associativity, int cache_id);
  ~LTP() {};

  void init(int num_sets, int assoc, int cache_id, bool log_enabled);

  LoggerLT logLT;

  void startTrace(int64_t cacheSet, int loc);
  void appendSignature(int64_t cacheSet, int loc, Addr PC);
  uint32_t  hashFunction(uint32_t A, uint32_t B);
  void deallocateSignature(int64_t cacheSet, int loc);
  void endTrace(int64_t cacheSet, int loc);
  bool checkLastTouch(int64_t cacheSet, int loc, Addr PC);
  void ltpTester();
  std::string print() const;
  std::string printHistoryTable() const;
  std::string printHistoryTable(int64_t cacheSet, int loc) const;
  std::string printSignatureTable() const;

private:
  int m_cache_num_sets;
  int m_cache_assoc;
  int m_cache_id;

  std::string logPrefix;

  //2-d vector of LTP Entries for signature table.
  //First index is number of sets. Second is associativity.
  std::vector<std::vector<ltpTrace *>> m_signature_table;

  //2-d vector of a set of LTP Entries for history table.
  std::vector<std::vector<std::set<ltpTrace *>>> m_history_table;
};

std::ostream& operator<<(std::ostream & out, const LTP& ltp);

#endif // __MEM_RUBY_STRUCTURES_LASTTOUCHPREDICTION_HH__
