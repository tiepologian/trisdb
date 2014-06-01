/* 
 * File:   TimeUtils.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 17 maggio 2014, 17.31
 */

#include "TimeUtils.h"
#include "Utils.h"

using namespace std::chrono;

double TimeUtils::getCurrentTimestamp() {
    milliseconds ms = duration_cast< milliseconds >(
            high_resolution_clock::now().time_since_epoch()
            );
    return std::chrono::duration_cast<milliseconds>(ms).count();
}

std::string TimeUtils::getTimeString() {
    std::time_t tt = system_clock::to_time_t (system_clock::now());
    std::string tmp = std::asctime(localtime(&tt));
    return Utils::rtrim(tmp);
}

std::string TimeUtils::getCustomTimeString() {
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char output[30];
    strftime(output, 30, "%d %b %Y %T", timeinfo);
    return std::string(output);
}