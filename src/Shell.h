/* 
 * File:   Shell.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 28 maggio 2014, 12.27
 */

#ifndef SHELL_H
#define	SHELL_H

#include "table_printer.h"
#include "Utils.h"
#include "TimeUtils.h"
#include "GenericClient.h"
#include "LogManager.h"
#ifdef __linux__
#include <readline/readline.h>
#include <readline/history.h>
#endif

class Shell {
public:
    Shell(std::string port, std::string socket);
    Shell(const Shell& orig);
    virtual ~Shell();
    void run();
    typedef boost::shared_ptr<QueryRequest> RequestPointer;
private:
    void quit();
    void printQueryResult(QueryResponse res, std::string cmd);
    std::string _port;
    std::string _socketPath;
    bool useSocket;
};

#endif	/* SHELL_H */