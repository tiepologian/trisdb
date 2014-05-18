/* 
 * File:   LogManager.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 17 maggio 2014, 22.16
 */

#include "LogManager.h"

LogManager* LogManager::inst_ = NULL;

LogManager* LogManager::getSingleton() {
    if(inst_ == NULL) {
        inst_ = new LogManager();
    }
    return inst_;
}

void LogManager::log(Severity s, std::string message) {
    std::string msg = TimeUtils::getTimeString();
    switch(s) {
        case 3:
            msg.append(" [INFO]: ");
            msg.append(message);
            std::cout << msg << std::endl;
            break;
        case 2:
            msg.append(" [WARNING]: ");
            msg.append(message);
            std::cout << msg << std::endl;
            break;
        case 1:
            msg.append(" [ERROR]: ");
            msg.append(message);
            std::cout << msg << std::endl;
            break;
        case 0:
            msg.append(" [CRITICAL]: ");
            msg.append(message);
            std::cout << msg << std::endl;
            break;
    }
}