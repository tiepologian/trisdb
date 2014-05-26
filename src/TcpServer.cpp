/*
 * File:   TcpServer.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 25 maggio 2014, 19.21
 */

#include "TcpServer.h"

using asio::ip::tcp;

const int max_length = 1024;

TcpServer::TcpServer(TrisDb* db) {
    this->_db = db;
}

TcpServer::TcpServer(const TcpServer& orig) {
    //
}

TcpServer::~TcpServer() {
    //
}

void TcpServer::session(tcp::socket sock) {
    try {
        for (;;) {
            char data[max_length];

            asio::error_code error;
            size_t length = sock.read_some(asio::buffer(data), error);
            if (error == asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw asio::system_error(error); // Some other error.
            asio::write(sock, asio::buffer(data, length));
            Message message;
            std::string ss(data);
            message.ParseFromString(ss);
            std::cout << "Message: " << message.query() << std::endl;
            double tms = message.timestamp();
            std::cout << "Timestamp: " << tms << std::endl;
        }

    } catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void TcpServer::server(asio::io_service& io_service, unsigned short port) {
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    for (;;) {
        tcp::socket sock(io_service);
        a.accept(sock);
        std::thread(&TcpServer::session, this, std::move(sock)).detach();
    }
}

void TcpServer::run() {
    try {
        asio::io_service io_service;
        server(io_service, 1205);
    } catch (std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }
}