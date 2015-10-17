/* 
 * File:   QueryParser.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 20 maggio 2014, 17.26
 */

#ifndef QUERYPARSER_H
#define	QUERYPARSER_H

#include "Utils.h"
#include "TimeUtils.h"

class QueryParser {
public:
    QueryParser();
    QueryParser(const QueryParser& orig);
    virtual ~QueryParser();
    struct Query {
        double timestamp;
        std::string command;
        Utils::record parameters;
	int limit = Utils::kQueryLimitWildcard;
    };
    // Receives string command, parses it and returns Query object
    void parse(std::string s, Query& q);
    //std::ostream& operator << (std::ostream &o, const Query &a);
    friend std::ostream& operator << (std::ostream &o, const Query &a);
private:
    void findLimit(std::string& s, Query& q);
};

#endif	/* QUERYPARSER_H */
