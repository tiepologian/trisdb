/* 
 * File:   TcpServer.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 25 maggio 2014, 19.21
 */

#ifndef TCPSERVER_H
#define	TCPSERVER_H

#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include "TrisDb.h"
#include "message.pb.h"
#include "GenericServer.h"

class TcpServer : public GenericServer {
public:
    TcpServer(TrisDb* db);
    TcpServer(const TcpServer& orig);
    virtual ~TcpServer();
    virtual void run();
    virtual void stop();
private:
    TrisDb* _db;
    void session(boost::asio::ip::tcp::socket sock);
    void server();
};

#endif	/* TCPSERVER_H */

