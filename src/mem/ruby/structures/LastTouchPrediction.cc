/**
 * @file CacheMemoryLtp.cc
 * @author Abhishek Bhaumick & Raghav V.
 * @brief
 * @version 0.1
 * @date 2021-04-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "mem/ruby/structures/CacheMemory.hh"

void LoggerLT::setup(int id);
{
    //  Append cache Id to Log name
    char fileName[20];
    sprintf(fileName, "LogLTP_%d.log", id);
    logFile.open(fileName, std::ofstream::out);
    if (logFile.is_open())
    std::cout << "trace: logFile - Open Succesful \n";
    else
    std::cout << "trace: logFile - Open failed \n";
}
