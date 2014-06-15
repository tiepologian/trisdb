/* 
 * File:   UnixSocketServer.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 14 giugno 2014, 19.36
 */

#ifndef UNIXSOCKETSERVER_H
#define	UNIXSOCKETSERVER_H

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
#include <atomic>
#include <memory>
#include <sys/stat.h>

class UnixSocketServer : public GenericServer {
public:
    UnixSocketServer(TrisDb* db, std::string path);
    UnixSocketServer(const UnixSocketServer& orig);
    virtual ~UnixSocketServer();
    virtual void run();
    virtual void stop();
    virtual int getOpenConnections();
    std::atomic<int> cnx = ATOMIC_VAR_INIT(0);
private:
    TrisDb* _db;
    std::string _path;
    boost::asio::io_service io_service;
    void server();
};

class AsyncUnixSocketServer {
public:
    AsyncUnixSocketServer(boost::asio::io_service& io_service, std::string filename, TrisDb* tris, UnixSocketServer* srv);
private:
    void do_accept();
    boost::asio::local::stream_protocol::acceptor acceptor_;    
    boost::asio::local::stream_protocol::socket socket_;
    TrisDb* _db;
    UnixSocketServer* _srv;
};

class AsyncUnixSocketSession : public std::enable_shared_from_this<AsyncUnixSocketSession> {
public:
    AsyncUnixSocketSession(boost::asio::local::stream_protocol::socket socket, TrisDb* tris, UnixSocketServer* srv);
    virtual ~AsyncUnixSocketSession();
    void start();
    typedef boost::shared_ptr<QueryRequest> RequestPointer;
    typedef boost::shared_ptr<QueryResponse> ResponsePointer;
private:
    TrisDb* _db;
    UnixSocketServer* _srv;
    void do_read();
    void parseMessage(unsigned size);
    void handle_read_header(const boost::system::error_code& error);
    void handle_read_body(const boost::system::error_code& error);
    boost::asio::local::stream_protocol::socket socket_;    
    std::vector<uint8_t> m_readbuf;
    PackedMessage<QueryRequest> m_packed_request;
};

#endif	/* UNIXSOCKETSERVER_H */