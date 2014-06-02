/* 
 * File:   TrisDb.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 18 maggio 2014, 14.50
 */

#include "TrisDb.h"
#include "LogManager.h"

volatile sig_atomic_t TrisDb::_terminateLoop = 0;

TrisDb::TrisDb(Config* config) {
    signal(SIGTERM, stop);
    this->_parser = new QueryParser();
    this->_planner = new QueryPlanner(this);
}

TrisDb::~TrisDb() {
    //
}

void TrisDb::addServer(GenericServer* g) {
    this->_servers.push_back(g);
}

void TrisDb::create(std::string a, std::string b, std::string c) {
    boost::lock_guard<boost::shared_mutex> lock(_mutex);
    dbData.add(a, b, c);
}

bool TrisDb::remove(std::string a, std::string b, std::string c) {
    boost::lock_guard<boost::shared_mutex> lock(_mutex);
    return dbData.remove(a, b, c);
}

Utils::ResultVector TrisDb::getFromA(std::string a) {
    boost::shared_lock<boost::shared_mutex> lock(_mutex);
    return dbData.getA(a);
}

Utils::ResultVector TrisDb::getFromB(std::string b) {
    boost::shared_lock<boost::shared_mutex> lock(_mutex);
    return dbData.getB(b);
}

Utils::ResultVector TrisDb::getFromC(std::string c) {
    boost::shared_lock<boost::shared_mutex> lock(_mutex);
    return dbData.getC(c);
}

Utils::ResultVector TrisDb::get(int index, std::string c) {
    boost::shared_lock<boost::shared_mutex> lock(_mutex);
    switch(index){
        case 0:
            return getFromA(c);
        case 1:
            return getFromB(c);
        case 2:
            return getFromC(c);
    }        
}

Utils::ResultVector TrisDb::getAll() {
    boost::shared_lock<boost::shared_mutex> lock(_mutex);
    return dbData.getAll();
}

void TrisDb::clearAll() {
    boost::lock_guard<boost::shared_mutex> lock(_mutex);
    dbData.clearAll();
}

QueryParser* TrisDb::getParser() {
    return _parser;
}

QueryPlanner* TrisDb::getPlanner() {
    return _planner;
}

void TrisDb::run() {
    // main thread calls run to start server
    // every run method creates a new thread, then main thread loops
    for(auto &i : this->_servers) {
        LogManager::getSingleton()->log(LogManager::LINFO, "Starting " + i->getServerName());        
        i->run();
    }
    // sleep
    LogManager::getSingleton()->log(LogManager::LINFO, "Database ready");
    while(!TrisDb::_terminateLoop) {
        std::chrono::milliseconds dura(2000);
        std::this_thread::sleep_for(dura);
    }
    // SIGTERM received, stop servers
    for(auto &i : this->_servers) {
        i->stop();
    }
    // give the other threads time to terminate
    sleep(2);
}

void TrisDb::stop(int param) {
    LogManager::getSingleton()->log(LogManager::LINFO, "Received signal SIGTERM");
    TrisDb::_terminateLoop = 1;
}