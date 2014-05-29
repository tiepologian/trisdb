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
#include "Utils.h"
#include "TrisDb.h"
#include "message.pb.h"

class TcpServer {
public:
    TcpServer(TrisDb* db);
    TcpServer(const TcpServer& orig);
    virtual ~TcpServer();
    void run();
private:
    TrisDb* _db;
    void session(boost::asio::ip::tcp::socket sock);
    void server(boost::asio::io_service& io_service, unsigned short port);
};

#endif	/* TCPSERVER_H */

