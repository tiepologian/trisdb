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
    this->_config = config;
    if (this->_config->getSetting("port") != "0") {
        // listen on TCP socket
        GenericServer* tcp = new TcpServer(this, std::atoi(_config->getSetting("port").c_str()));
        this->addServer(tcp);
    }
    if (this->_config->getSetting("unixsocket") != "no") {
        // listen on Unix socket
        GenericServer* uds = new UnixSocketServer(this, this->_config->getSetting("unixsocket"));
        this->addServer(uds);
    }
}

TrisDb::~TrisDb() {
    for (auto &i : this->_servers) {
        delete i;
    }
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
    switch (index) {
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

Utils::ResultVector TrisDb::count() {
    boost::shared_lock<boost::shared_mutex> lock(_mutex);
    return dbData.getCount();
}

Utils::ResultVector TrisDb::status() {
    boost::shared_lock<boost::shared_mutex> lock(_mutex);
    Utils::ResultVector result;
    auto tmp = dbData.getCount();
    result.push_back(tmp.at(0));
    double vm, rss;
    Utils::process_mem_usage(vm, rss);
    rss /= 1024.0;
    result.push_back(std::make_tuple("memory", "value", std::to_string(rss) + " MB"));
    int connections = 0;
    for (auto &i : this->_servers) {
        connections += i->getOpenConnections();
    }
    result.push_back(std::make_tuple("connections", "value", std::to_string(connections)));
    return result;
}

void TrisDb::clearAll() {
    boost::lock_guard<boost::shared_mutex> lock(_mutex);
    dbData.clearAll();
}

void TrisDb::benchmark() {
    LogManager::getSingleton()->log(LogManager::LINFO, "Starting benchmark");
    double now_1 = TimeUtils::getCurrentTimestamp();
    for (int i = 0; i < 1000000; i++) {
        create("Key" + std::to_string(i), "Value" + std::to_string(i), Utils::toString(i));
        if (i % 100000 == 0) {
            std::cout << std::to_string(i) << std::endl;
        }
    }
    double now_2 = TimeUtils::getCurrentTimestamp();
    double tempo = now_2 - now_1;
    double speed = 1000000.0 / (tempo / 1000.0);
    LogManager::getSingleton()->log(LogManager::LINFO, "Writing 1,000,000 triples took " + std::to_string(tempo) + "ms");
    LogManager::getSingleton()->log(LogManager::LINFO, "Write Speed: " + std::to_string(speed) + "/s");

    double now_3 = TimeUtils::getCurrentTimestamp();
    for (int i = 0; i < 1000000; i++) {
        Utils::ResultVector res = getFromA("Key" + std::to_string(i));
        if (i % 100000 == 0) {
            std::cout << std::to_string(i) << std::endl;
        }
    }
    double now_4 = TimeUtils::getCurrentTimestamp();
    double tempo_2 = now_4 - now_3;
    double speed_2 = 1000000.0 / (tempo_2 / 1000.0);
    LogManager::getSingleton()->log(LogManager::LINFO, "Reading 1,000,000 triples took " + std::to_string(tempo_2) + "ms");
    LogManager::getSingleton()->log(LogManager::LINFO, "Read Speed: " + std::to_string(speed_2) + "/s");
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
    for (auto &i : this->_servers) {
        LogManager::getSingleton()->log(LogManager::LINFO, "Starting " + i->getServerName());
        i->run();
    }
    // sleep
    LogManager::getSingleton()->log(LogManager::LINFO, "Database ready");
    while (!TrisDb::_terminateLoop) {
        std::chrono::milliseconds dura(2000);
        std::this_thread::sleep_for(dura);
    }
    // SIGTERM received, stop servers
    for (auto &i : this->_servers) {
        i->stop();
    }
    // give the other threads time to terminate
    // TODO:    this isn't very secure because if there are queries going on it
    //          could take longer to close TCP or Unix Socket connections.
    //          We should wait for a message/signal from the threads
    sleep(2);
    StorageEngine::getSingleton()->syncSave(&this->dbData, this->_config->getSetting("dbfolder"));
}

void TrisDb::stop(int param) {
    LogManager::getSingleton()->log(LogManager::LINFO, "Received signal SIGTERM");
    TrisDb::_terminateLoop = 1;
}