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

class TrisDb {
public:
    TrisDb(Config* config);
    virtual ~TrisDb();
    void create(std::string a, std::string b, int c);
    Utils::ResultVector getFromA(std::string a);
    Utils::ResultVector getFromB(std::string b);
    Utils::ResultVector getFromC(int c);
private:
    TripleMap<std::string, std::string, int> dbData;
};

#endif	/* TRISDB_H */