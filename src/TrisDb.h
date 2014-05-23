/* 
 * File:   TrisDb.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 18 maggio 2014, 14.50
 */

#ifndef TRISDB_H
#define	TRISDB_H

#include "TripleMap.h"
#include "Config.h"
#include "QueryPlanner.h"

class TrisDb {
public:
    TrisDb(Config* config);
    virtual ~TrisDb();
    void create(std::string a, std::string b, std::string c);
    bool remove(std::string a, std::string b, std::string c);
    Utils::ResultVector getFromA(std::string a);
    Utils::ResultVector getFromB(std::string b);
    Utils::ResultVector getFromC(std::string c);
    Utils::ResultVector get(int index, std::string c);
    Utils::ResultVector getAll();
    void clearAll();
    QueryParser* getParser();
    QueryPlanner* getPlanner();
private:
    TripleMap<std::string, std::string, std::string> dbData;
    QueryParser* _parser;
    QueryPlanner* _planner;
};

#endif	/* TRISDB_H */