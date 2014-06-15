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
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <ostream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <vector>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <exception>
#include <unordered_set>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#define TRISDB_VERSION "0.0.3"

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
    typedef std::tuple<std::string, std::string, std::string> record;
    typedef std::vector<record> ResultVector;
    static const std::string kQueryWildcard = "***";

    static const std::unordered_set<std::string> ValidCommands{"CREATE", "GET", "GETS", "GETP", "GETO", "DELETE", "CLEAR", "COUNT", "STATUS", "QUIT"};

    template<typename T>
    std::string toString(T t) {
        return boost::lexical_cast<std::string>(t);
    }

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

    inline void printAsciiLogo() {
        std::cout << " _____     _       ____  ____  " << std::endl;
        std::cout << "|_   _| __(_)___  |  _ \\| __ ) " << std::endl;
        std::cout << "  | || '__| / __| | | | |  _ \\ " << "   " << TRISDB_VERSION_STR << std::endl;
        std::cout << "  | || |  | \\__ \\ | |_| | |_) |" << "   PID: " << getpid() << std::endl;
        std::cout << "  |_||_|  |_|___/ |____/|____/ " << std::endl;
        std::cout << std::endl;
    }

    inline void process_mem_usage(double& vm_usage, double& resident_set) {
        using std::ios_base;
        using std::ifstream;
        using std::string;

        vm_usage = 0.0;
        resident_set = 0.0;

        // 'file' stat seems to give the most reliable results
        //
        ifstream stat_stream("/proc/self/stat", ios_base::in);

        // dummy vars for leading entries in stat that we don't care about
        //
        string pid, comm, state, ppid, pgrp, session, tty_nr;
        string tpgid, flags, minflt, cminflt, majflt, cmajflt;
        string utime, stime, cutime, cstime, priority, nice;
        string O, itrealvalue, starttime;

        // the two fields we want
        //
        unsigned long vsize;
        long rss;

        stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
                >> utime >> stime >> cutime >> cstime >> priority >> nice
                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

        stat_stream.close();

        long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
        vm_usage = vsize / 1024.0;
        resident_set = rss * page_size_kb;
    }    

    class CustomException : std::exception {
    public:
        virtual const char* what() const throw () {
            return "Syntax Error in command!";
        }
    };
}

#endif	/* UTILS_H */