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
    AsyncTcpServer s(io_service, 1205, this->_db, this);
    // 2nd thread blocks here
    io_service.run();
}

int TcpServer::getOpenConnections() {
    return cnx;
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

AsyncTcpServer::AsyncTcpServer(boost::asio::io_service& io_service, short port, TrisDb* tris, TcpServer* srv) : acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), socket_(io_service) {
    do_accept();
    this->_db = tris;
    this->_srv = srv;
}

void AsyncTcpServer::do_accept() {
    acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
        if (!ec) {
            std::make_shared<AsyncTcpSession>(std::move(socket_), this->_db, this->_srv)->start();
        }
        do_accept();
    });
}

AsyncTcpSession::AsyncTcpSession(boost::asio::ip::tcp::socket socket, TrisDb* tris, TcpServer* srv) : socket_(std::move(socket)), m_packed_request(boost::shared_ptr<QueryRequest>(new QueryRequest())) {
    this->_db = tris;
    this->_srv = srv;
    this->_srv->cnx.fetch_add(1);
}

AsyncTcpSession::~AsyncTcpSession() {
    this->_srv->cnx.fetch_sub(1);
}

void AsyncTcpSession::start() {
    do_read();
}

void AsyncTcpSession::handle_read_header(const boost::system::error_code& error) {
    if (!error) {
        unsigned msg_len = m_packed_request.decode_header(m_readbuf);
        parseMessage(msg_len);
    }
}

void AsyncTcpSession::do_read() {
    auto self(shared_from_this());
    // get message length from header
    m_readbuf.resize(HEADER_SIZE);
    boost::asio::async_read(socket_, boost::asio::buffer(m_readbuf),
            boost::bind(&AsyncTcpSession::handle_read_header, shared_from_this(),
            boost::asio::placeholders::error));
}

void AsyncTcpSession::parseMessage(unsigned msg_len) {
    m_readbuf.resize(HEADER_SIZE + msg_len);
    boost::asio::mutable_buffers_1 buf = boost::asio::buffer(&m_readbuf[HEADER_SIZE], msg_len);
    boost::asio::async_read(socket_, buf,
            boost::bind(&AsyncTcpSession::handle_read_body, shared_from_this(),
            boost::asio::placeholders::error));
}

void AsyncTcpSession::handle_read_body(const boost::system::error_code& error) {
    if (!error) {
        if (m_packed_request.unpack(m_readbuf)) {
            RequestPointer req = m_packed_request.get_msg();

            QueryParser::Query q;
            Utils::ResultVector result;
            QueryParser* parser = this->_db->getParser();
            QueryPlanner* planner = this->_db->getPlanner();
            for (int i = 0; i < req->query_size(); i++) {                
                parser->parse(req->query(i), q);         
                result = planner->execute(q);               
            }           
            ResponsePointer resp = boost::make_shared<QueryResponse>();
            double queryTime = TimeUtils::getCurrentTimestamp() - q.timestamp;
            resp->set_timestamp(std::to_string(queryTime));

            for (Utils::ResultVector::const_iterator it = result.begin(); it != result.end(); ++it) {
                QueryResponse::Record* rec = resp->add_data();
                rec->set_subject(std::get<0>(*it));
                rec->set_predicate(std::get<1>(*it));
                rec->set_object(std::get<2>(*it));
            }
            std::vector<google::protobuf::uint8> writebuf;
            PackedMessage<QueryResponse> resp_msg(resp);
            resp_msg.pack(writebuf);
            auto self(shared_from_this());
            boost::asio::async_write(socket_, boost::asio::buffer(writebuf),
                    [this, self](boost::system::error_code ec, std::size_t length) {
                        if (!ec) {
                            do_read();
                        }
                    }
            );
        }
    }
}