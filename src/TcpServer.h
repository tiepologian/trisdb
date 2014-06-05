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
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "TrisDb.h"
#include "message.pb.h"
#include "GenericServer.h"
#include "PackedMessage.h"
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
    typedef boost::shared_ptr<QueryRequest> RequestPointer;
    typedef boost::shared_ptr<QueryResponse> ResponsePointer;
private:
    TrisDb* _db;
    void do_read();
    void do_write(ResponsePointer res);
    void parseMessage(unsigned size);
    void handle_read_header(const boost::system::error_code& error);
    void handle_read_body(const boost::system::error_code& error);
    boost::asio::ip::tcp::socket socket_;
    enum {
        max_length = 1024
    };
    char data_[max_length];
    std::vector<uint8_t> m_readbuf;
    PackedMessage<QueryRequest> m_packed_request;
};

#endif	/* TCPSERVER_H */