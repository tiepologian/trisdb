/*
 * File:   TcpServer.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 25 maggio 2014, 19.21
 */

#include "TcpServer.h"
#include "LogManager.h"

using boost::asio::ip::tcp;
const int max_length = 1024;

TcpServer::TcpServer(TrisDb* db) {
    this->_db = db;
    this->_serverName = "Tcp Server";
}

TcpServer::TcpServer(const TcpServer& orig) {
    //
}

TcpServer::~TcpServer() {
    //
}

void TcpServer::server() {
    LogManager::getSingleton()->log(LogManager::LINFO, "Listening for TCP connections on port 1205");
    AsyncTcpServer s(io_service, 1205, this->_db);
    // 2nd thread blocks here
    io_service.run();
}

void TcpServer::run() {
    try {
        std::cout.sync_with_stdio(true);
        std::thread(&TcpServer::server, this).detach();
    } catch (std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }
}

void TcpServer::stop() {
    LogManager::getSingleton()->log(LogManager::LINFO, "Stopping TCP Server");
    // stop io_service so 2nd thread terminates
    io_service.stop();
}

AsyncTcpServer::AsyncTcpServer(boost::asio::io_service& io_service, short port, TrisDb* tris) : acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), socket_(io_service) {
    do_accept();
    this->_db = tris;
}

void AsyncTcpServer::do_accept() {
    acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
        if (!ec) {
            std::make_shared<AsyncTcpSession>(std::move(socket_), this->_db)->start();
        }
        do_accept();
    });
}

AsyncTcpSession::AsyncTcpSession(boost::asio::ip::tcp::socket socket, TrisDb* tris) : socket_(std::move(socket)) {
    this->_db = tris;
}

void AsyncTcpSession::start() {
    do_read();
}

void AsyncTcpSession::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length), [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            // de-serialize data, exec query and write result
            QueryRequest req;
            std::string ss(data_);
            req.ParseFromString(ss);

            QueryParser::Query q = this->_db->getParser()->parse(req.query());
            Utils::ResultVector result = this->_db->getPlanner()->execute(q);
            QueryResponse res;
            double queryTime = TimeUtils::getCurrentTimestamp() - q.timestamp;
            res.set_timestamp(std::to_string(queryTime));
            for (Utils::ResultVector::const_iterator it = result.begin(); it != result.end(); ++it) {
                QueryResponse::Record* rec = res.add_data();
                rec->set_subject(std::get<0>(*it));
                rec->set_predicate(std::get<1>(*it));
                rec->set_object(std::get<2>(*it));
            }           

            do_write(res);
        }
    });
}

void AsyncTcpSession::do_write(QueryResponse res) {
    auto self(shared_from_this());
    boost::asio::streambuf b;
    std::ostream os(&b);
    res.SerializeToOstream(&os);    
    boost::asio::async_write(socket_, b, [this, self](boost::system::error_code ec, std::size_t) {
        //socket_.close();
        if (!ec) {
            do_read();
        }
    });
}