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

void LoggerLT::setup(const char * prefix, int id)
{
    this->prefix = prefix;

    //  Append cache Id to Log name
    char fileName[20];
    sprintf(fileName, "LogLTP_%d.log", id);
    logFile.open(fileName, std::ofstream::out);
    if (logFile.is_open())
        std::cout << "trace: logFile " << id << " - Open Succesful \n";
    else
        std::cout << "trace: logFile " << id << " - Open Failed \n";
}

std::ostream& operator<<(std::ostream & out, const ltpTrace & t)
{
    out << t.valid << " ";
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
LTP::init (int num_sets, int assoc, int cache_id)
{
    m_cache_num_sets = num_sets;
    m_cache_assoc = assoc;
    m_cache_id = cache_id;

    logPrefix = "";
    logPrefix = "trace " + std::to_string(m_cache_id) + " : ";
    logLT.setup(logPrefix.c_str(), m_cache_id);
    traceLog(logLT, "log setup done \n");

    m_signature_table.resize(m_cache_num_sets,
                    std::vector<ltpTrace*>(m_cache_assoc, nullptr));
    m_history_table.resize(m_cache_num_sets,
                    std::vector<std::set<ltpTrace*>>(m_cache_assoc)); //verify

    traceLog(logLT, "resized to %d \n", m_cache_num_sets);
    ltpTester();
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
    assert(signature == NULL || signature == nullptr);

    signature = new ltpTrace;
    //
    // signature->PCVector.push_back(PC);
    signature->valid = true;

    m_signature_table[cacheSet][loc] = signature;
    traceLog(logLT, "alloc sig %x with PC '%016x' to [%010d,%04d] \n",
        signature, cacheSet, loc);
}
void LTP::appendSignature(int64_t cacheSet, int loc, Addr PC)
{
    ltpTrace *signature = m_signature_table[cacheSet][loc];
    assert(signature->valid = true);

    signature->PCVector.push_back(PC);
    traceLog(logLT, "Added PC '%16x' to [set,idx] = [%010d,%04d] Trace \n",
        PC, cacheSet, loc);
}
void LTP::deallocateSignature(int64_t cacheSet, int loc)
{
    ltpTrace *signature = m_signature_table[cacheSet][loc];
    if (signature != NULL) {
        delete (signature);
        m_signature_table[cacheSet][loc] = nullptr;
    }
}

void LTP::endTrace(int64_t cacheSet, int loc)
{
    ltpTrace *completedSignature = m_signature_table[cacheSet][loc];

    traceLog(logLT, "endTrace [%010d,%04d]\n",
        cacheSet, loc);
    //Allocate a signature in history table and copy the ltpTrace
    if (completedSignature == NULL || completedSignature == nullptr) {
        traceLog(logLT, "endTrace [%010d,%04d] -- null warn\n",
            cacheSet, loc);
    }
    else {
        ltpTrace *signature = new ltpTrace;
        // traceLog(logLT, "endTrace vector size %d \n",
        //     completedSignature->PCVector.size());
        // signature->PCVector = completedSignature->PCVector;
        m_history_table[cacheSet][loc].insert(signature);

        //deallocate completed signature
        deallocateSignature(cacheSet, loc);
    }
}

//  TODO: modify to accept the request PC
//          check if match AFTER appending incoming PC
// bool LTP::checkLastTouch_unused(int cacheSet, int loc)
// {
//     std::set<ltpTrace*> traceHistory = m_history_table[cacheSet][loc];
//     //check if current signature is in history.
//     return (traceHistory.find(m_signature_table[cacheSet][loc])
//             != traceHistory.end());
// }

bool
LTP::checkLastTouch(int64_t cacheSet, int loc, Addr PC) //TODO
{
    //  std::set<ltpTrace*> traceHistory = m_history_table[cacheSet][loc];
    // //check if current signature is in history.

    // ltpTrace tempSignature= *m_signature_table[cacheSet][loc];
    // assert(tempSignature.valid);

    // tempSignature.PCVector.push_back(PC);


    // return (traceHistory.find(tempSignature)
    //         != traceHistory.end());

    return false;
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
    ss << "------------- \n";
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

