/* 
 * File:   UnixSocketClient.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 14 giugno 2014, 20.20
 */

#ifndef UNIXSOCKETCLIENT_H
#define	UNIXSOCKETCLIENT_H

#include "GenericClient.h"

class UnixSocketClient : public GenericClient {
public:
    UnixSocketClient(std::string path);
    virtual ~UnixSocketClient();
    virtual QueryResponse connect(RequestPointer resp);
private:
    std::shared_ptr<boost::asio::local::stream_protocol::socket> _s;
};

#endif	/* UNIXSOCKETCLIENT_H */
