/* 
 * File:   TrisDb.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 18 maggio 2014, 14.50
 */

#include "TrisDb.h"

TrisDb::TrisDb(Config* config) {
    this->_parser = new QueryParser();
    this->_planner = new QueryPlanner(this);
}

TrisDb::~TrisDb() {
}

void TrisDb::create(std::string a, std::string b, std::string c) {
    dbData.add(a, b, c);
}

bool TrisDb::remove(std::string a, std::string b, std::string c) {
    return dbData.remove(a, b, c);
}

Utils::ResultVector TrisDb::getFromA(std::string a) {
    return dbData.getA(a);
}

Utils::ResultVector TrisDb::getFromB(std::string b) {
    return dbData.getB(b);
}

Utils::ResultVector TrisDb::getFromC(std::string c) {
    return dbData.getC(c);
}

Utils::ResultVector TrisDb::get(int index, std::string c) {
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
    return dbData.getAll();
}

void TrisDb::clearAll() {
    dbData.clearAll();
}

QueryParser* TrisDb::getParser() {
    return _parser;
}

QueryPlanner* TrisDb::getPlanner() {
    return _planner;
}