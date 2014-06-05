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
    try {
        boost::asio::io_service io_service;
        _s = std::make_shared<boost::asio::ip::tcp::socket>(io_service);
        tcp::resolver resolver(io_service);
        boost::asio::connect(*_s, resolver.resolve({"localhost", "1205"}));
    } catch (std::exception& e) {
        LogManager::getSingleton()->log(LogManager::LCRITICAL, "Unable to connect to database on localhost:1205");
        exit(1);
    }
}

TcpClient::~TcpClient() {
    _s->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    _s->close();
    _s.reset();
}

QueryResponse TcpClient::connect(QueryRequest req) {
    try {
        boost::asio::streambuf b;
        std::ostream os(&b);
        req.SerializeToOstream(&os);
        boost::asio::write(*_s, b);

        char reply[1024];
        boost::system::error_code error;

        size_t length = _s->read_some(boost::asio::buffer(reply), error);

        QueryResponse res;
        res.ParseFromString(reply);
        return res;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}