/* 
 * File:   GenericClient.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 15 giugno 2014, 18.54
 */

#ifndef GENERICCLIENT_H
#define	GENERICCLIENT_H

#include <boost/asio.hpp>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "message.pb.h"
#include "Utils.h"
#include "PackedMessage.h"

class GenericClient {
public:
    GenericClient() {};
    virtual ~GenericClient() {};
    typedef boost::shared_ptr<QueryRequest> RequestPointer;
    typedef boost::shared_ptr<QueryResponse> ResponsePointer;
    virtual QueryResponse connect(RequestPointer resp) = 0;    
};

#endif	/* GENERICCLIENT_H */

