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
    TcpClient(const TcpClient& orig);
    virtual ~TcpClient();
    QueryResponse connect(QueryRequest req);
    static bool checkConnection();
private:
    int max_length = 1024;    
};

#endif	/* TCPCLIENT_H */