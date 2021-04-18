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

  void setup(const char * prefix, int id);

  ~LoggerLT()
  {
    if (logFile.is_open())
      logFile.close();
  }

protected:
  std::ofstream logFile;
  void log(const Loc &loc, std::string s) override
  {
    logFile << s << std::flush;
  }
};

#define traceLog(logger, ...) \
  logger.print(::LoggerLT::Loc(__FILE__, __LINE__), __VA_ARGS__)

struct ltpTrace
{
  std::vector<Addr> PCVector;
  bool valid;
};

class LTP
{

public:
  LTP() {};
  LTP(int numberOfSets, int associativity, int cache_id);
  ~LTP() {};

  void init(int num_sets, int assoc, int cache_id);

  LoggerLT logLT;

  void allocateSignature(int cacheSet, int loc, Addr PC);
  void appendSignature(int cacheSet, int loc, Addr PC);
  void deallocateSignature(int cacheSet, int loc);
  void endTrace(int cacheSet, int loc);
  bool checkLastTouch(int cacheSet, int loc);

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

#endif // __MEM_RUBY_STRUCTURES_LASTTOUCHPREDICTION_HH__
