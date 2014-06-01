/*
 * File:   TcpClient.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 28 maggio 2014, 15.48
 */

#include "TcpClient.h"
#include "LogManager.h"

using boost::asio::ip::tcp;

TcpClient::TcpClient() {
    //
}

TcpClient::TcpClient(const TcpClient& orig) {
    //
}

TcpClient::~TcpClient() {
    //
}

QueryResponse TcpClient::connect(QueryRequest req) {
    try {
        boost::asio::io_service io_service;
        tcp::socket s(io_service);
        tcp::resolver resolver(io_service);
        boost::asio::connect(s, resolver.resolve({"localhost", "1205"}));

        boost::asio::streambuf b;
        std::ostream os(&b);
        req.SerializeToOstream(&os);
        boost::asio::write(s, b);

        char reply[1024];
        boost::system::error_code error;

        size_t length = s.read_some(boost::asio::buffer(reply), error);

        s.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        QueryResponse res;
        res.ParseFromString(reply);
        return res;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

bool TcpClient::checkConnection() {
    try {
        boost::asio::io_service io_service;
        tcp::socket s(io_service);
        tcp::resolver resolver(io_service);
        boost::asio::connect(s, resolver.resolve({"localhost", "1205"}));
        s.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        return true;
    } catch (std::exception& e) {
        LogManager::getSingleton()->log(LogManager::LCRITICAL, "Unable to connect to database on localhost:1205");
        exit(1);
        return false;
    }
}