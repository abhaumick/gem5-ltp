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
        std::cout << "trace: logFile - Open Succesful \n";
    else
        std::cout << "trace: logFile - Open failed \n";
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

    m_signature_table.resize(num_sets);
    m_history_table.resize(assoc);

}

void LTP::allocateSignature(int cacheSet, int loc, Addr PC)
{
    ltpTrace *signature = m_signature_table[cacheSet][loc];
    assert(signature == NULL);

    signature = new ltpTrace;
    signature->PCVector.push_back(PC);
    signature->valid = true;

    m_signature_table[cacheSet][loc] = signature;
}
void LTP::appendSignature(int cacheSet, int loc, Addr PC)
{
    ltpTrace *signature = m_signature_table[cacheSet][loc];
    assert(signature != NULL);

    signature->PCVector.push_back(PC);
}
void LTP::deallocateSignature(int cacheSet, int loc)
{
    ltpTrace *signature = m_signature_table[cacheSet][loc];
    assert(signature != NULL);
    delete (signature);
}

void LTP::endTrace(int cacheSet, int loc)
{
    ltpTrace *completedSignature = m_signature_table[cacheSet][loc];

    //Allocate a signature in history table and copy the ltpTrace
    ltpTrace *signature = new ltpTrace;
    signature->PCVector = completedSignature->PCVector;
    m_history_table[cacheSet][loc].insert(signature);

    //deallocate completed signature
    deallocateSignature(cacheSet, loc);
}

bool LTP::checkLastTouch(int cacheSet, int loc)
{
    std::set<ltpTrace*> traceHistory = m_history_table[cacheSet][loc];
    //check if current signature is in history.
    return (traceHistory.find(m_signature_table[cacheSet][loc])
            != traceHistory.end());
}
