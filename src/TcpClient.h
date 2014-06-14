/* 
 * File:   TcpClient.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 28 maggio 2014, 15.47
 */

#ifndef TCPCLIENT_H
#define	TCPCLIENT_H

#include <boost/asio.hpp>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "message.pb.h"
#include "Utils.h"
#include "PackedMessage.h"

class TcpClient {
public:
    TcpClient();
    virtual ~TcpClient();    
    typedef boost::shared_ptr<QueryRequest> RequestPointer;
    typedef boost::shared_ptr<QueryResponse> ResponsePointer;
    QueryResponse connect(RequestPointer resp);
private:
    std::shared_ptr<boost::asio::ip::tcp::socket> _s;
};

#endif	/* TCPCLIENT_H */