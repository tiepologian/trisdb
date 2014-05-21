/* 
 * File:   TrisDb.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 18 maggio 2014, 14.50
 */

#include "TrisDb.h"

TrisDb::TrisDb(Config* config) {
    this->_parser = new QueryParser();
}

TrisDb::~TrisDb() {
}

void TrisDb::create(std::string a, std::string b, std::string c) {
    dbData.add(a, b, c);
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

QueryParser* TrisDb::getParser() {
    return _parser;
}