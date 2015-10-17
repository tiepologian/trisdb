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

void QueryParser::parse(std::string s, Query& q) {
    q.timestamp = TimeUtils::getCurrentTimestamp();
    findLimit(s, q);

    unsigned pos = s.find(" ");
    q.command = boost::to_upper_copy(s.substr(0, pos));

    // is the command valid?
    Utils::CustomException ex;
    if(Utils::ValidCommands.find(q.command) == Utils::ValidCommands.end()) throw ex;

    if (q.command == "QUIT" || q.command == "CLEAR" || q.command == "COUNT" || q.command == "STATUS") return;

    std::vector<std::string>params;

    // run this only if there are characters after the command
    if((pos > 0) && (pos <= s.length())) {
        std::regex words_regex("\"([^\"]*)\"");
        auto words_begin = std::sregex_iterator(s.begin(), s.end(), words_regex);
        auto words_end = std::sregex_iterator();

        for (auto i=words_begin;i != words_end;++i) {
            std::string match = (*i).str();
	    match = match.substr(1, match.length()-2);
	    if(match == "*") params.push_back(Utils::kQueryWildcard);
            else params.push_back(match);
        }
    }

    // are there enough argsuments..?
    if(params.size() == 0) {
	if(q.command == "CREATE") throw ex;
	else q.parameters = std::make_tuple(Utils::kQueryWildcard, Utils::kQueryWildcard, Utils::kQueryWildcard);
	return;
    } else if (params.size() < 2) {
	// only first argument, assume wildcard for second and third
        if(q.command == "CREATE") throw ex;
        else q.parameters = std::make_tuple(params.at(0), Utils::kQueryWildcard, Utils::kQueryWildcard);
        return;
    } else if(params.size() < 3) {
	// only first two arguments, assume wildcard for third
	if(q.command == "CREATE") throw ex;
	else q.parameters = std::make_tuple(params.at(0), params.at(1), Utils::kQueryWildcard);
	return;
    }

    q.parameters = std::make_tuple(params.at(0), params.at(1), params.at(2));
    return;
}

void QueryParser::findLimit(std::string& s, Query& q) {
    std::size_t found = ((s.rfind("LIMIT") != std::string::npos) ? s.rfind("LIMIT") : s.rfind("limit"));
    if(found != std::string::npos) {
	std::string substring = s.substr(found);
	std::size_t foundSpace = substring.find(" ");
	std::size_t foundEnd = substring.find(" ", foundSpace+1);
        std::string limitString = substring.substr(foundSpace+1, foundEnd-(foundSpace+1));
	q.limit = atoi(limitString.c_str());
	s.erase(found, foundEnd);
    }
}

std::ostream& operator <<(std::ostream &o, const QueryParser::Query &a) {
    o << "TIMESTAMP: " << a.timestamp << std::endl;
    o << "COMMAND: " << a.command << std::endl;
    o << "PARAMS: " << std::get<0>(a.parameters) << "-" << std::get<1>(a.parameters) << "-" << std::get<2>(a.parameters) << std::endl;
    return o;
}
