/* 
 * File:   TimeUtils.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 17 maggio 2014, 17.31
 */

#include "TimeUtils.h"

using namespace std::chrono;

double TimeUtils::getCurrentTimestamp() {
    milliseconds ms = duration_cast< milliseconds >(
            high_resolution_clock::now().time_since_epoch()
            );
    return std::chrono::duration_cast<milliseconds>(ms).count();
}