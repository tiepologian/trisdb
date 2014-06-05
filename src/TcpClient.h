/* 
 * File:   TcpClient.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 28 maggio 2014, 15.47
 */

#ifndef TCPCLIENT_H
#define	TCPCLIENT_H

#include <boost/asio.hpp>
#include "message.pb.h"
#include "Utils.h"

class TcpClient {
public:
    TcpClient();
    virtual ~TcpClient();
    QueryResponse connect(QueryRequest req);
private:
    int max_length = 1024;    
    std::shared_ptr<boost::asio::ip::tcp::socket> _s;
};

#endif	/* TCPCLIENT_H */