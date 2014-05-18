/* 
 * File:   TrisDb.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 18 maggio 2014, 14.50
 */

#include "TrisDb.h"

TrisDb::TrisDb(Config* config) {
}

TrisDb::~TrisDb() {
}

void TrisDb::create(std::string a, std::string b, int c) {
    dbData.add(a, b, c);
}

std::tuple<std::string, std::string, int> TrisDb::getFromA(std::string a) {
    return dbData.getA(a);
}

std::tuple<std::string, std::string, int> TrisDb::getFromB(std::string b) {
    return dbData.getB(b);
}