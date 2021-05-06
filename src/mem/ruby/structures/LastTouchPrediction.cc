/**
 * @file LastTouchPRediction.cc
 * @author Abhishek Bhaumick & Raghav V.
 * @brief
 * @version 0.1
 * @date 2021-04-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "mem/ruby/structures/LastTouchPrediction.hh"

void LoggerLT::setup(const char * prefix, int id, bool enabled)
{
  this->prefix = prefix;
  this->enabled = enabled;

  if (enabled) {
    //  Append cache Id to Log name
    char fileName[20];
    sprintf(fileName, "LogLTP_%d.log", id);
    logFile.open(fileName, std::ofstream::out);
    if (logFile.is_open())
      std::cout << "trace: logFile " << id << " - Open Succesful \n";
    else
      std::cout << "trace: logFile " << id << " - Open Failed \n";
  }
  else {
    std::cout << "trace: logFile " << id << " - Disabled! \n";
  }
}

std::ostream& operator<<(std::ostream & out, const ltpTrace & t)
{
  out << "ltpTrace : " << t.valid << " ";
  if (t.valid) {
    for (auto pc : t.PCVector)
      out << std::hex << pc << " -> ";
  }
  out << " ";
  return out;
}

LTP::LTP(int num_sets, int assoc, int cache_id)
  :   logLT("trace : "),
  m_cache_num_sets(num_sets),
  m_cache_assoc(assoc),
  m_cache_id(cache_id)
{}

void
LTP::init (int num_sets, int assoc, int cache_id, bool log_enabled)
{
  m_cache_num_sets = num_sets;
  m_cache_assoc = assoc;
  m_cache_id = cache_id;

  logPrefix = "";
  logPrefix = "trace " + std::to_string(m_cache_id) + " : ";
  logLT.setup(logPrefix.c_str(), m_cache_id, log_enabled);
  traceLog(logLT, "log setup done \n");

  m_signature_table.resize(m_cache_num_sets,
          std::vector<ltpTrace*>(m_cache_assoc, nullptr));
  m_history_table.resize(m_cache_num_sets,
          std::vector<std::set<ltpTrace*>>(m_cache_assoc));

  traceLog(logLT, "resized to %d \n", m_cache_num_sets);
  //ltpTester();
}

/**
 * @brief allocates a new ltp trace with empty trace
 *
 * @param cacheSet
 * @param loc
 */
void LTP::startTrace(int64_t cacheSet, int loc)
{
  ltpTrace *signature = m_signature_table[cacheSet][loc];
  // assert(signature == NULL || signature == nullptr);
  if (signature == NULL || signature == nullptr) {
    signature = new ltpTrace;
    signature->valid = false;
    m_signature_table[cacheSet][loc] = signature;

    traceLog(logLT, "kalm  : startTrace sig %x to [%010d,%04d] \n",
      signature, cacheSet, loc);
  }
  else {
    traceLog(logLT, "panik : startTrace sig %x to [%010d,%04d] exists\n",
      signature, cacheSet, loc);
  }
}

/**
 * @brief
 *
 * @param cacheSet
 * @param loc
 * @param PC
 */
void LTP::appendSignature(int64_t cacheSet, int loc, Addr PC)
{
  ltpTrace *signature = m_signature_table[cacheSet][loc];
  if (signature == nullptr || signature == NULL) {
    traceLog(logLT, "panik : appendSig : [%010d,%04d] = null \n",
      cacheSet, loc);
  }
  else {
    //signature->PCVector.push_back(PC);
    signature->hash = hashFunction(signature->hash, PC);
    signature->valid = true;
    traceLog(logLT, "kalm  : appendSig PC '%16x' to [%010d,%04d] \n",
      PC, cacheSet, loc);
  }
}

/**
 * @brief
 *
 * @param cacheSet
 * @param loc
 */
void LTP::deallocateSignature(int64_t cacheSet, int loc)
{
  ltpTrace *signature = m_signature_table[cacheSet][loc];
  if (signature == NULL || signature == nullptr) {
    traceLog(logLT, "panik : deallocateSig : [%010d,%04d] null \n",
      cacheSet, loc);
    DPRINTF(RubySlicc, "panik : deallocateSig : [%010d,%04d] null \n",
      cacheSet, loc);
  }
  else {
    traceLog(logLT, "kalm  : deallocateSig : [%010d,%04d] \n",
      cacheSet, loc);
    DPRINTF(RubySlicc, "kalm  : deallocateSig : [%010d,%04d] \n",
      cacheSet, loc);
    delete (signature);
    m_signature_table[cacheSet][loc] = nullptr;
  }
}

/**
 * @brief
 *
 *
 * @param cacheSet
 * @param loc
 */
bool LTP::endTrace(int64_t cacheSet, int loc)
{
  if (GLOBAL) {
    cacheSet = 0;
    loc = 0;
  }

  ltpTrace *completedSignature = m_signature_table[cacheSet][loc];
  bool isPresent = false;
  bool newTraceAdded = false;
  //Allocate a signature in history table and copy the ltpTrace
  if (completedSignature == NULL || completedSignature == nullptr) {
    traceLog(logLT, "panik : endTrace [%010d,%04d] -- null warn\n",
      cacheSet, loc);
  }
  else {
    if (completedSignature->valid)
    {
      for (auto histTrace : m_history_table[cacheSet][loc]) {
        if (histTrace == NULL || histTrace == nullptr) {
          continue;
        }
        else {
          if (histTrace->valid) {
            // True-> simply increment pred counter
            if (histTrace->hash
              == completedSignature->hash)
            {
               if (histTrace->predCount < CONFIDENCE_COUNT)
              {
                histTrace->predCount++;
              }
              isPresent = true;
              traceLog(logLT,
              "End trace match. Pred counter: %d \n",
              histTrace->predCount);
            }

          }
        }
      }
      //False -> then make a new signature and insert in history table
      if (!isPresent)
      {
        ltpTrace *signature = new ltpTrace;
        // traceLog(logLT, "New endTrace vector size %d \n",
        // completedSignature->PCVector.size());
        // signature->PCVector = completedSignature->PCVector;
        signature->hash = completedSignature->hash;
        signature->valid = true;
        m_history_table[cacheSet][loc].insert(signature);
        traceLog(logLT, printHistoryTable(cacheSet, loc));
        newTraceAdded = true;
      }
    }
    //deallocate completed signature
    traceLog(logLT, "kalm  : endTrace [%010d,%04d]\n",
      cacheSet, loc);
    DPRINTF(RubySlicc, "kalm  : endTrace [%010d,%04d]\n",
      cacheSet, loc);
    deallocateSignature(cacheSet, loc);
  }
  return newTraceAdded;
}

bool
LTP::checkLastTouch(int64_t cacheSet, int loc, Addr PC)
{
   if (GLOBAL) {
    cacheSet = 0;
    loc = 0;
  }

  std::set<ltpTrace*> traceHistory = m_history_table[cacheSet][loc];
  //check if current signature is in history.
  ltpTrace tempSignature;
  bool PCFlag = false;
  bool hashFlag = false;
  int matchSize = -1;
  int hashMatchSize = -1;
  uint32_t matchedHash = -1;

  if (m_signature_table[cacheSet][loc] != NULL
    && m_signature_table[cacheSet][loc] != nullptr )
  {
    tempSignature = *m_signature_table[cacheSet][loc];
    //tempSignature.PCVector.push_back(PC);
    tempSignature.hash = hashFunction(tempSignature.hash, PC);

    //  Find matching trace in history
    for (auto histTrace : m_history_table[cacheSet][loc]) {
      if (histTrace == NULL || histTrace == nullptr) {
        continue;
      }
      else {
        if (histTrace->valid) {
          // if (histTrace->PCVector == tempSignature.PCVector &&
          //   histTrace->predCount == CONFIDENCE_COUNT ) {
          //   PCFlag = true;
          //   matchSize = histTrace->PCVector.size();
          // }
          if (histTrace->hash == tempSignature.hash &&
            histTrace->predCount == CONFIDENCE_COUNT) {
            hashFlag = true;
            //hashMatchSize = tempSignature.PCVector.size();
            matchedHash = tempSignature.hash;
          }
        }
      }
    }
  }
  else
  {
    traceLog(logLT, "panik : checkLastTouch [%010d,%04d] -- null warn\n",
      cacheSet, loc);
    DPRINTF(RubySlicc, "panik : checkLastTouch [%010d,%04d] -- null warn\n",
      cacheSet, loc);
  }

  if (hashFlag)
  {
    traceLog(logLT, "kalm  : PC Touch Match : %d - size %d , \
      Hash Touch Match : %d = %x for [%010d,%04d] of size %d \n",
      PCFlag, matchSize, hashFlag, matchedHash,
      cacheSet, loc, hashMatchSize);
    DPRINTF(RubySlicc, "kalm  : PC Touch Match : %d - size %d , \
      Hash Touch Match : %d = %x for [%010d,%04d] of size %d \n",
      PCFlag, matchSize, hashFlag, matchedHash,
      cacheSet, loc, hashMatchSize);
  }
  else
  {
    //traceLog(logLT, "kalm  : Last Touch No Match Found! \n");
  }

  return hashFlag;
}


uint32_t  LTP::hashFunction(uint32_t A, uint32_t B)
{
  return A + B; //TODO : Implement truncated signature
}

void LTP::ltpTester()
{
  traceLog(logLT, "\n -- LTP Self Test -- \n");
  //verify the sizes of both data structures
  char buffer[50];
  sprintf(buffer, "Size of history table is %lu x %lu\n",
    m_history_table.size(), m_history_table[0].size());
  traceLog(logLT, buffer);

  sprintf(buffer, "Size of signature table is %lu x %lu\n",
    m_signature_table.size(), m_signature_table[0].size());
  traceLog(logLT, buffer);

  int cacheSet, loc;
  Addr PC;

  //allocate a signature
  cacheSet = 0;
  loc = 1;
  PC = 0x0A;
  startTrace(cacheSet, loc);

  //append a signature
  PC = 0xBD;
  appendSignature(cacheSet, loc, PC);

  //end the current trace
  endTrace(cacheSet, loc);

  //check the last touch
  checkLastTouch(cacheSet, loc, PC);


  traceLog(logLT, "\n -- End of LTP Self Test -- \n");
  traceLog(logLT, this->print());
  traceLog(logLT, "\n -- End of LTP Self Test -- \n");


}

std::string LTP::print() const
{
  std::stringstream ss;
  ss << "\n";
  ss << "Last-Touch Predictor \n";
  // std::cerr << this->printHistoryTable();
  ss << this->printHistoryTable();
  // std::cerr << this->printSignatureTable();
  ss << this->printSignatureTable();
  ss << "\n\n";

  return ss.str();
}

std::string LTP::printHistoryTable() const
{
  std::stringstream ss;
  ss << "History Table \n";
  ss << "------------- \n";
  for (auto set = 0; set < m_cache_num_sets; ++ set) {
    for (auto idx = 0; idx < m_cache_assoc; ++ idx) {

    }
  }
  ss << "------------- \n";
  return ss.str();
}

std::string LTP::printHistoryTable(int64_t cacheSet, int loc) const
{
  std::stringstream ss;

  ss << "History Table : [" << cacheSet << "," << loc << "] \n";
  auto hist = m_history_table[cacheSet][loc];
  for (auto item : hist) {
    ss << * item << " \n";
  }
  return ss.str();
}

std::string LTP::printSignatureTable() const
{
  std::stringstream ss;
  ss << "Signature Table \n";
  ss << "------------- \n";

  for (auto set = 0; set < m_cache_num_sets; ++ set) {
    for (auto idx = 0; idx < m_cache_assoc; ++ idx) {
      ltpTrace * trace = m_signature_table[set][idx];
      // std::cerr << "[" << set << ", " << idx << "] --> "
      //     << trace << (trace == nullptr) << " \n";
      if (trace == nullptr || trace == NULL) {
        // ss << " * undef * " << " \n";
      }
      else {
        ss << "[" << set << ", " << idx << "] --> ";
        ss << * trace << " \n";
      }
    }
  }

  ss << "------------- \n";
  return ss.str();
}

std::ostream& operator<<(std::ostream & out, const LTP & ltp)
{
  return (out << ltp.print());
}

