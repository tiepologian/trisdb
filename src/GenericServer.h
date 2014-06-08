/* 
 * File:   GenericServer.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 1 giugno 2014, 10.39
 */

#ifndef GENERICSERVER_H
#define	GENERICSERVER_H

#include "Utils.h"

class GenericServer {
public:
    GenericServer() {};
    virtual ~GenericServer() {};
    virtual void run() = 0;
    virtual void stop() = 0;
    virtual int getOpenConnections() = 0;
    virtual std::string getServerName() {
        return _serverName;
    }
protected:
    std::string _serverName;
};

#endif	/* GENERICSERVER_H */