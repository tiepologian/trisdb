/* 
 * File:   TrisDb.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 18 maggio 2014, 14.50
 */

#ifndef TRISDB_H
#define	TRISDB_H

#include <thread>
#include <boost/thread/shared_mutex.hpp>
#include <signal.h>
#include "TripleMap.h"
#include "Config.h"
#include "QueryPlanner.h"
#include "GenericServer.h"
#include "TimeUtils.h"
#include "TcpServer.h"
#include "UnixSocketServer.h"
#include "StorageEngine.h"

class TrisDb {
public:
    TrisDb(Config* config);
    virtual ~TrisDb();
    void addServer(GenericServer* g);
    void run();
    void create(std::string a, std::string b, std::string c);
    bool remove(std::string a, std::string b, std::string c);
    Utils::ResultVector getFromA(std::string a);
    Utils::ResultVector getFromB(std::string b);
    Utils::ResultVector getFromC(std::string c);
    Utils::ResultVector get(int index, std::string c);
    Utils::ResultVector getAll();
    Utils::ResultVector count();
    Utils::ResultVector status();
    void clearAll();
    QueryParser* getParser();
    QueryPlanner* getPlanner();
    static void stop(int param);
    void benchmark();
private:
    TripleMap<std::string, std::string, std::string> dbData;
    QueryParser* _parser;
    QueryPlanner* _planner;
    Config* _config;
    std::vector<GenericServer*> _servers;
    static volatile sig_atomic_t _terminateLoop;
    boost::shared_mutex _mutex;
};

#endif	/* TRISDB_H */