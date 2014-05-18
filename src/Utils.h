/* 
 * File:   Utils.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 17 maggio 2014, 18.54
 */

#ifndef UTILS_H
#define	UTILS_H

#include <iostream>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <vector>
#include <iomanip>
#include <iterator>
#include <sstream>

#define TRISDB_VERSION "0.0.1"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#if defined __clang__
#define COMPILER "CLANG " __clang_version__
#elif defined __GNUC__
#define COMPILER "GCC " STR(__GNUC__) "." STR(__GNUC_MINOR__) "." STR(__GNUC_PATCHLEVEL__)
#else
#define COMPILER "UNKNOWN COMPILER"
#endif

#ifndef DEBUG
#define TRISDB_VERSION_STR "TrisDB " TRISDB_VERSION " (" COMPILER ")"
#else
#define TRISDB_VERSION_STR "TrisDB " TRISDB_VERSION " (DEBUG)" " (" COMPILER ")"
#endif

namespace Utils {
    typedef std::vector<std::tuple<std::string, std::string, int>> ResultVector;

    static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    static void split(const std::string& str, std::vector<std::string>& v) {
        std::stringstream ss(str);
        ss >> std::noskipws;
        std::string field;
        char ws_delim;
        while (1) {
            if (ss >> field) v.push_back(field);
            else if (ss.eof())
                break;
            else
                v.push_back(std::string());
            ss.clear();
            ss >> ws_delim;
        }
    }
}

#endif	/* UTILS_H */