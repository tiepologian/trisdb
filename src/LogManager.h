/* 
 * File:   LogManager.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 17 maggio 2014, 22.16
 */

#ifndef LOGMANAGER_H
#define	LOGMANAGER_H

#include "Utils.h"
#include "TimeUtils.h"

class LogManager {
public:
    enum Severity {
        LINFO = 3,
        LWARNING = 2,
        LERROR = 1,
        LCRITICAL = 0
    };
    static LogManager* getSingleton();
    static void log(Severity s, std::string message);    
private:
    static LogManager* inst_;
    LogManager() {};
    LogManager(const LogManager&) {};
    LogManager& operator=(const LogManager&);
};

#endif	/* LOGMANAGER_H */

