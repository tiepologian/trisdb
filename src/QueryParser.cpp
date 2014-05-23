/* 
 * File:   QueryParser.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 20 maggio 2014, 17.26
 */

#include "QueryParser.h"

QueryParser::QueryParser() {
    //
}

QueryParser::QueryParser(const QueryParser& orig) {
    //
}

QueryParser::~QueryParser() {
    //
}

QueryParser::Query QueryParser::parse(std::string s) {
    Query q;
    q.timestamp = TimeUtils::getCurrentTimestamp();
    unsigned pos = s.find(" ");
    q.command = boost::to_upper_copy(s.substr(0, pos));
    
    // is the command valid?
    Utils::CustomException ex;
    if(Utils::ValidCommands.find(q.command) == Utils::ValidCommands.end()) throw ex;

    if (q.command == "QUIT" || q.command == "CLEAR") return q;

    std::vector<std::string>params;

    Tokenizer tok(s.substr(pos + 1), "\"");
    while (tok.NextToken()) {
        if (tok.GetToken() != " ") {
            if(tok.GetToken() == "*") params.push_back(Utils::kQueryWildcard);
            else params.push_back(tok.GetToken());
        }
    }

    // are there enough argsuments..?
    if (params.size() < 3) {
        if(q.command == "CREATE") throw ex;
        // only first value in get, assume wildcards for other two
        else q.parameters = std::make_tuple(params.at(0), Utils::kQueryWildcard, Utils::kQueryWildcard);
        return q;
    }
        
    q.parameters = std::make_tuple(params.at(0), params.at(1), params.at(2));
    return q;
}

std::ostream& operator <<(std::ostream &o, const QueryParser::Query &a) {
    o << "TIMESTAMP: " << a.timestamp << std::endl;
    o << "COMMAND: " << a.command << std::endl;
    o << "PARAMS: " << std::get<0>(a.parameters) << "-" << std::get<1>(a.parameters) << "-" << std::get<2>(a.parameters) << std::endl;
    return o;
}