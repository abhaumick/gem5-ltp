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

#include "base/logging.hh"
#include "mem/ruby/structures/CacheMemory.hh"

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
    LoggerLT(const char* prefix) : Logger(prefix)
    {
      // logFile.open("LogLTP.log", std::ofstream::out);
    }

    void setup(int id);

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


class LTP



#endif // __MEM_RUBY_STRUCTURES_LASTTOUCHPREDICTION_HH__
