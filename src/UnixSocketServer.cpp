/* 
 * File:   UnixSocketServer.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 14 giugno 2014, 19.36
 */

#include "UnixSocketServer.h"
#include "LogManager.h"

using boost::asio::ip::tcp;

UnixSocketServer::UnixSocketServer(TrisDb* db, std::string path) {
    this->_db = db;
    this->_serverName = "Unix Domain Socket";
    this->_path = path;
}

UnixSocketServer::UnixSocketServer(const UnixSocketServer& orig) {
    //
}

UnixSocketServer::~UnixSocketServer() {
    //
}

void UnixSocketServer::server() {
    LogManager::getSingleton()->log(LogManager::LINFO, "Listening for Unix Domain connections on " + this->_path);
    std::remove(this->_path.c_str());
    AsyncUnixSocketServer s(io_service, this->_path, this->_db, this);
    chmod(this->_path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    // 2nd thread blocks here
    io_service.run();
}

int UnixSocketServer::getOpenConnections() {
    return cnx;
}

void UnixSocketServer::run() {
    try {
        std::cout.sync_with_stdio(true);
        std::thread(&UnixSocketServer::server, this).detach();
    } catch (std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }
}

void UnixSocketServer::stop() {
    LogManager::getSingleton()->log(LogManager::LINFO, "Stopping Unix Domain Socket");
    // stop io_service so 2nd thread terminates
    io_service.stop();
}

AsyncUnixSocketServer::AsyncUnixSocketServer(boost::asio::io_service& io_service, std::string filename, TrisDb* tris, UnixSocketServer* srv) : acceptor_(io_service, boost::asio::local::stream_protocol::endpoint(filename)), socket_(io_service) {
    do_accept();
    this->_db = tris;
    this->_srv = srv;
}

void AsyncUnixSocketServer::do_accept() {
    acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
        if (!ec) {
            std::make_shared<AsyncUnixSocketSession>(std::move(socket_), this->_db, this->_srv)->start();
        }
        do_accept();
    });
}

AsyncUnixSocketSession::AsyncUnixSocketSession(boost::asio::local::stream_protocol::socket socket, TrisDb* tris, UnixSocketServer* srv) : socket_(std::move(socket)), m_packed_request(boost::shared_ptr<QueryRequest>(new QueryRequest())) {
    this->_db = tris;
    this->_srv = srv;
    this->_srv->cnx.fetch_add(1);
}

AsyncUnixSocketSession::~AsyncUnixSocketSession() {
    this->_srv->cnx.fetch_sub(1);
}

void AsyncUnixSocketSession::start() {
    do_read();
}

void AsyncUnixSocketSession::handle_read_header(const boost::system::error_code& error) {
    if (!error) {
        unsigned msg_len = m_packed_request.decode_header(m_readbuf);
        parseMessage(msg_len);
    }
}

void AsyncUnixSocketSession::do_read() {
    auto self(shared_from_this());
    // get message length from header
    m_readbuf.resize(HEADER_SIZE);
    boost::asio::async_read(socket_, boost::asio::buffer(m_readbuf),
            boost::bind(&AsyncUnixSocketSession::handle_read_header, shared_from_this(),
            boost::asio::placeholders::error));
}

void AsyncUnixSocketSession::parseMessage(unsigned msg_len) {
    m_readbuf.resize(HEADER_SIZE + msg_len);
    boost::asio::mutable_buffers_1 buf = boost::asio::buffer(&m_readbuf[HEADER_SIZE], msg_len);
    boost::asio::async_read(socket_, buf,
            boost::bind(&AsyncUnixSocketSession::handle_read_body, shared_from_this(),
            boost::asio::placeholders::error));
}

void AsyncUnixSocketSession::handle_read_body(const boost::system::error_code& error) {
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