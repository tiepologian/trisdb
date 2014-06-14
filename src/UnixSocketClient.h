/* 
 * File:   UnixSocketClient.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 14 giugno 2014, 20.20
 */

#ifndef UNIXSOCKETCLIENT_H
#define	UNIXSOCKETCLIENT_H

#include <boost/asio.hpp>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "message.pb.h"
#include "Utils.h"
#include "PackedMessage.h"

class UnixSocketClient {
public:
    UnixSocketClient();
    virtual ~UnixSocketClient();
    typedef boost::shared_ptr<QueryRequest> RequestPointer;
    typedef boost::shared_ptr<QueryResponse> ResponsePointer;
    QueryResponse connect(RequestPointer resp);
private:
    std::shared_ptr<boost::asio::local::stream_protocol::socket> _s;
};

#endif	/* UNIXSOCKETCLIENT_H */