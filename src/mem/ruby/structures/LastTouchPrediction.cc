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

LTP::LTP(int num_sets, int assoc, int cache_id)
    :   logLT("trace : "),
    m_cache_num_sets(num_sets),
    m_cache_assoc(assoc),
    m_cache_id(cache_id)
{}

void
LTP::init ()
{
    logPrefix = "";
    logPrefix = "trace " + std::to_string(m_cache_id) + " : ";
    logLT.setup(logPrefix.c_str(), m_cache_id);
    traceLog(logLT, "log setup done \n");

    m_signature_table.resize(m_cache_num_sets);
    m_history_table.resize(m_cache_num_sets);
    traceLog(logLT, "resized to %d \n", m_cache_num_sets);

}

void LTP::allocateSignature(int cacheSet, int loc, Addr PC)
{
    LtpTrace *signature = m_signature_table[cacheSet][loc];
    assert(signature == NULL);

    signature = new LtpTrace;
    signature->PCVector.push_back(PC);
    signature->valid = true;

    m_signature_table[cacheSet][loc] = signature;
}
void LTP::appendSignature(int cacheSet, int loc, Addr PC)
{
    LtpTrace *signature = m_signature_table[cacheSet][loc];
    assert(signature != NULL);

    signature->PCVector.push_back(PC);
}
void LTP::deallocateSignature(int cacheSet, int loc)
{
    LtpTrace *signature = m_signature_table[cacheSet][loc];
    assert(signature != NULL);
    delete (signature);
}

void LTP::endTrace(int cacheSet, int loc)
{
    LtpTrace *completedSignature = m_signature_table[cacheSet][loc];

    //Allocate a signature in history table and copy the LtpTrace
    LtpTrace *signature = new LtpTrace;
    signature->PCVector = completedSignature->PCVector;
    m_history_table[cacheSet][loc].insert(signature);

    //deallocate completed signature
    deallocateSignature(cacheSet, loc);
}
