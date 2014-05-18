/* 
 * File:   TimeUtils.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 17 maggio 2014, 17.31
 */

#ifndef TIMEUTILS_H
#define	TIMEUTILS_H

#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>

class TimeUtils {
public:
    TimeUtils();
    static double getCurrentTimestamp();
    static std::string getTimeString();
};

#endif	/* TIMEUTILS_H */