/* 
 * File:   UnixSocketClient.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 14 giugno 2014, 20.20
 */

#include "UnixSocketClient.h"
#include "LogManager.h"

using boost::asio::local::stream_protocol;

UnixSocketClient::UnixSocketClient(std::string path) {
    try {
        boost::asio::io_service io_service;
        _s = std::make_shared<boost::asio::local::stream_protocol::socket>(io_service);
        _s->connect(boost::asio::local::stream_protocol::endpoint(path));        
    } catch (std::exception& e) {
        LogManager::getSingleton()->log(LogManager::LCRITICAL, "Unable to connect to database on socket "+path);
        exit(1);
    }
}

UnixSocketClient::~UnixSocketClient() {
    _s->shutdown(boost::asio::local::stream_protocol::socket::shutdown_both);
    _s->close();
    _s.reset();
}

QueryResponse UnixSocketClient::connect(RequestPointer resp) {
    try {
        std::vector<google::protobuf::uint8> writebuf;
        PackedMessage<QueryRequest> resp_msg(resp);
        resp_msg.pack(writebuf);
        boost::asio::write(*_s, boost::asio::buffer(writebuf));

        boost::system::error_code error;
        std::vector<uint8_t> m_readbuf;
        ResponsePointer resp2 = boost::make_shared<QueryResponse>();
        PackedMessage<QueryResponse> res_msg(resp2);

        m_readbuf.resize(HEADER_SIZE);
        boost::asio::read(*_s, boost::asio::buffer(m_readbuf), boost::asio::transfer_exactly(HEADER_SIZE), error);
        unsigned msg_len = res_msg.decode_header(m_readbuf);
        m_readbuf.resize(HEADER_SIZE + msg_len);
        boost::asio::mutable_buffers_1 buf = boost::asio::buffer(&m_readbuf[HEADER_SIZE], msg_len);
        boost::asio::read(*_s, buf, boost::asio::transfer_exactly(HEADER_SIZE + msg_len), error);
        if (res_msg.unpack(m_readbuf)) {
            ResponsePointer res = res_msg.get_msg();
            return *res;
        }
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}