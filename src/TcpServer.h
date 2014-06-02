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
#include <cstdlib>
#include <memory>

class TcpServer : public GenericServer {
public:
    TcpServer(TrisDb* db);
    TcpServer(const TcpServer& orig);
    virtual ~TcpServer();
    virtual void run();
    virtual void stop();
private:
    TrisDb* _db;
    boost::asio::io_service io_service;
    void server();
};

class AsyncTcpServer {
public:
    AsyncTcpServer(boost::asio::io_service& io_service, short port, TrisDb *tris);
private:
    void do_accept();
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
    TrisDb* _db;
};

class AsyncTcpSession : public std::enable_shared_from_this<AsyncTcpSession> {
public:
    AsyncTcpSession(boost::asio::ip::tcp::socket socket, TrisDb* tris);
    void start();
private:
    TrisDb* _db;
    void do_read();
    void do_write(QueryResponse res);
    boost::asio::ip::tcp::socket socket_;
    enum {
        max_length = 1024
    };
    char data_[max_length];
};

#endif	/* TCPSERVER_H */