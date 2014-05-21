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
       
    boost::char_separator<char> sep("\"", "\"");
    boost::tokenizer<boost::char_separator<char> > tokens(s.substr(pos+1), sep);   
    
    std::vector<std::string>params;
    for (tokenizer::iterator tok_iter = tokens.begin();
            tok_iter != tokens.end(); ++tok_iter) {
        if (*tok_iter != " ") {
            if(*tok_iter == "*") {
                params.push_back(*tok_iter);
            }
            else {
                params.push_back(*tok_iter);
            }
        }
    }
    // are there enough argsuments..?
    Utils::CustomException ex;
    if(params.size() < 3) throw ex;
    q.parameters = std::make_tuple(params.at(0), params.at(1), params.at(2));
    return q;
}

std::ostream& operator <<(std::ostream &o, const QueryParser::Query &a) {
    o << "TIMESTAMP: " << a.timestamp << std::endl;
    o << "COMMAND: " << a.command << std::endl;
    o << "PARAMS: " << std::get<0>(a.parameters) << "-" << std::get<1>(a.parameters) << "-" << std::get<2>(a.parameters) << std::endl;
    return o;
}