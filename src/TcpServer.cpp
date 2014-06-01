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

void TcpServer::session(tcp::socket sock) {
    // !!! THIS IS RUN IN A NEW THREAD !!!
    try {
        for (;;) {
            char data[max_length];

            //asio::error_code error;
            boost::system::error_code error;
            size_t length = sock.read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.            
            

            QueryRequest req;
            std::string ss(data);
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
            
            boost::asio::streambuf b;
            std::ostream os(&b);
            res.SerializeToOstream(&os);
            boost::asio::write(sock, b);
        }

    } catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void TcpServer::server() {    
    LogManager::getSingleton()->log(LogManager::LINFO, "Listening for TCP connections on port 1205");
    boost::asio::io_service io_service;
    unsigned short port = 1205;
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    for (;;) {
        tcp::socket sock(io_service);
        a.accept(sock);
        std::thread(&TcpServer::session, this, std::move(sock)).detach();
    }
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
}