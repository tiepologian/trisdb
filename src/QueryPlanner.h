/* 
 * File:   QueryPlanner.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 21 maggio 2014, 21.52
 */

#ifndef QUERYPLANNER_H
#define	QUERYPLANNER_H

#include "QueryParser.h"
#include <deque>

class TrisDb;

class QueryPlanner {
public:
    QueryPlanner(TrisDb* parent);
    QueryPlanner(const QueryPlanner& orig);
    virtual ~QueryPlanner();
    Utils::ResultVector execute(QueryParser::Query q);
private:
    TrisDb* _parent;
    std::deque<int> getQueryPlan(QueryParser::Query q);
    bool isEqual(std::string s1, std::string s2);
};

#endif	/* QUERYPLANNER_H */

