/* 
 * File:   TcpClient.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 28 maggio 2014, 15.47
 */

#ifndef TCPCLIENT_H
#define	TCPCLIENT_H

#include "GenericClient.h"
#include "SimpleTimer.h"

class TcpClient : public GenericClient {
public:
    TcpClient(std::string port);
    virtual ~TcpClient();    
    virtual QueryResponse connect(RequestPointer resp);
private:
    std::shared_ptr<boost::asio::ip::tcp::socket> _s;
};

#endif	/* TCPCLIENT_H */
