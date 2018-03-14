#ifndef _RTDB_CLIENT_IMPL_HEADER_H_
#define _RTDB_CLIENT_IMPL_HEADER_H_
#include "protocol/packet_protocol.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "api/rtdb_client.h"

class rtdb_client::rtdb_client_impl
{
public: 
        rtdb_client_impl(const char *ipaddr,const int port);
        ~rtdb_client_impl();
        
        int connect();
        int disconnect();
        void close();

protected:
        int send_packet(FIELD *fields,int len);
        int recv_packet(FIELD *fields,int len);
private:
        boost::asio::ip::tcp::endpoint m_endpoint;
        boost::asio::io_service m_io_client;
        boost::asio::ip::tcp::socket m_socket;
};
#endif
