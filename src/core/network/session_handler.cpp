#include "core/network/session_handler.h"
#include "common/input_stream.h"
#include "common/output_stream.h"
#include "common/io_streambuf.h"
#include "core/network/async_acceptor.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <assert.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/make_shared.hpp> 
#define LOG4Z_FORMAT_INPUT_ENABLE
#include "libs/log4z.h"

/////////////////////////////////////////////////////////////////
session_handler::session_handler(boost::asio::io_service& io_service,async_acceptor *acceptor)
:m_socket(io_service)
,m_acceptor(acceptor)
,m_header_size(0)
{
	//caculate packet header bytes size
	MSG_COMMON_REQ_FIELDS(MSG_COMMON,obj);
	fields_size(obj,0,&m_header_size,0,0);
}

session_handler::~session_handler()
{
}

void session_handler::start()
{
	boost::asio::ip::tcp::endpoint remote_addr=m_socket.remote_endpoint();
	LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID,"Remote client connect!,IP:%s:%d",remote_addr.address().to_string().c_str(),remote_addr.port());

	m_recv_buffer.clear();
	m_recv_buffer.resize(m_header_size);
	m_socket.async_read_some(boost::asio::buffer(&m_recv_buffer[0],m_header_size),
			boost::bind(&session_handler::handle_read, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred,
				m_header_size)
			);
}

void session_handler::close()
{
	boost::asio::ip::tcp::endpoint remote_addr=m_socket.remote_endpoint();
	LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID,"Disconnect!,Client IP:%s:%d",
			remote_addr.address().to_string().c_str(),remote_addr.port());

	m_acceptor->remove_session_handler(this);

	delete this;
}

void session_handler::handle_read(const boost::system::error_code& error,size_t bytes_transferred,size_t want_bytes)
{
	if (!error)
	{
		if(bytes_transferred<=0)
		{
			close();
		}
		else if(bytes_transferred < want_bytes )
		{
			//recv data for next partment
			//size_t base=m_recv_buffer.
			m_socket.async_read_some(boost::asio::buffer(&m_recv_buffer[bytes_transferred],want_bytes-bytes_transferred),
					boost::bind(&session_handler::handle_read, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred,
						want_bytes-bytes_transferred)
					);
		}
		else
		{
			char *p=&m_recv_buffer[0];
			int32_t mark=*(reinterpret_cast<int32_t *>(p));
			if( HEAD_MARK==mark )
			{
				handle_packet_header();
			}
			else if ( BODY_MARK==mark)
			{
				handle_packet_body();
			}
			else
			{
				LOGFMT_ERROR(LOG4Z_MAIN_LOGGER_ID,"Unknow packet mark 0x%08x",mark);
			}
		}
	}	
	else
	{
		close();
	}
}

void session_handler::handle_write(const boost::system::error_code& error,size_t bytes_transferred)
{
	if (!error)
	{
		m_socket.async_read_some(boost::asio::buffer(&m_recv_buffer[0], m_header_size),
				boost::bind(&session_handler::handle_read, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred,
					m_header_size));
	}
	else
	{
		close();
	}
}

int session_handler::handle_packet_header()
{
	MSG_COMMON_REQ_FIELDS(MSG_COMMON,robj);
	copy_fields(m_packet_header,robj,true);

	int ret=read_packet_header();
	if(0==ret)
	{
		int body_size=m_packet_header[FIELD_BSIZE].ivalue;

		m_recv_buffer.clear();
		m_recv_buffer.resize(body_size);
		m_socket.async_read_some(boost::asio::buffer(&m_recv_buffer[0],body_size),
				boost::bind(&session_handler::handle_read, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred,
					body_size)
				);
		
	}
	else
	{
		LOGFMT_ERROR(LOG4Z_MAIN_LOGGER_ID,"Error in function read_packet_header(),return code: 0x%08x",ret);
	}

	return ret;
}

int session_handler::read_packet_header()
{
	istreambuf b((char *)(&m_recv_buffer[0]),m_header_size);
	input_stream rd(&b);
	return unserialize_header_packet(rd,m_packet_header,true);	
}

int session_handler::handle_packet_body()
{
	int method =m_packet_header[FIELD_METHOD].ivalue;
	switch(method)
	{
	case MSG_CONNECT:
		return msg_connect();
	case MSG_DISCONNECT:
		return msg_disconnect();
	default:
		return -1;
	}
}

int session_handler::read_packet_body(FIELD *fields,int len)
{
	copy_fields(fields,m_packet_header,true);
	int body_size=m_packet_header[FIELD_BSIZE].ivalue;
	
	istreambuf b(&m_recv_buffer[0],body_size);
        input_stream rd(&b);
        return unserialize_body_packet(rd,fields,len,true);	
}

int session_handler::send_packet(FIELD *fields,int len)
{
	int total=0,header=0,body=0,body_count=0;
	fields_size(fields,&total,&header,&body,&body_count);

	fields[FIELD_BSIZE].ivalue=body;

	m_send_buffer.clear();
	m_send_buffer.resize(total);	

	ostreambuf b(&m_send_buffer[0],total);
	output_stream wd(&b);
	int ret=serialize_packet(wd,fields,len);
	if(0==ret)
	{
		boost::asio::async_write(m_socket,
				boost::asio::buffer(&m_send_buffer[0], total),
				boost::bind(&session_handler::handle_write, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		LOGFMT_ERROR(LOG4Z_MAIN_LOGGER_ID,"Error in function encode_packet(),return code: 0x%08x",ret);
	}
	return ret;	
}
