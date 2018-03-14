#include "api/rtdb_client_impl.h"
#include "api/rtdb_client.h"
#include "common/input_stream.h"
#include "common/output_stream.h"
#include "common/io_streambuf.h"
#include "protocol/packet_protocol.h"
#include "protocol/protocol_network.h"
#include <vector>
#include <iostream>
#include <unistd.h>

rtdb_client::rtdb_client_impl::rtdb_client_impl(const char *ipaddr,const int port)
:m_endpoint(boost::asio::ip::address::from_string(ipaddr),port)
,m_io_client()
,m_socket(m_io_client)
{

}

rtdb_client::rtdb_client_impl::~rtdb_client_impl()
{
}

/////////////////////////////////////////////////////////////////
//Start API functions

int rtdb_client::rtdb_client_impl::connect()
{
	boost::system::error_code err;
	m_socket.open(boost::asio::ip::tcp::v4()); 
	m_socket.connect(m_endpoint,err);
	if(err)
	{
		std::cerr<<"Error:"<<err.message()<<std::endl;
		return -1;
	}

        int ret=0;
        char computer[1024]={"client"};
        char process[1024]={"test/hhh/client"};

	gethostname(computer,1024);
	readlink("/proc/self/exe", process,1024);

        {
                MSG_CONNECT_FIELDS(computer,process,wobj);
                ret=send_packet(wobj,FIELDS_LEN(wobj));
        }

        if(0!=ret) return ret;

        {
                MSG_CONNECT_RESP_FIELDS(0,0,robj);
                ret=recv_packet(robj,FIELDS_LEN(robj));
                if(ret!=ERR_OK)
                        return ret;
        }

        return ret;
}

int rtdb_client::rtdb_client_impl::disconnect()
{
	MSG_DISCONNECT_FIELDS(wobj);
	int ret=send_packet(wobj,FIELDS_LEN(wobj));

	if(0!=ret)
		return ret;

	return 0;
}

void rtdb_client::rtdb_client_impl::close()
{
	//m_socket.shutdown();
}

//End API functions
//////////////////////////////////////////////////////////////

int rtdb_client::rtdb_client_impl::send_packet(FIELD *fields,int len)
{
	int total=0,header=0,body=0,body_count=0;
	fields_size(fields,&total,&header,&body,&body_count);
	fields[FIELD_BSIZE].ivalue=body;

	std::vector<char> send_buffer;
	send_buffer.resize(total);

	ostreambuf b(&send_buffer[0],total);
        output_stream wd(&b);
	int ret=serialize_packet(wd,fields,len);
	if(0==ret)
	{
		boost::system::error_code ec;
		size_t write_bytes=m_socket.write_some(boost::asio::buffer(&send_buffer[0],total), ec);
		if(write_bytes<(size_t)total || ec)
		{
			//std::cerr<<"Send data failed ,error:"<<ec.message()<<std::endl;
			return ERR_SEND_PACKET_ERROR;
		}
	}

	return ret;
}


int rtdb_client::rtdb_client_impl::recv_packet(FIELD *fields,int len)
{
	int header_size=0;
	fields_size(fields,0,&header_size,0,0);
	
	std::vector<char> recv_header_buffer;
        recv_header_buffer.resize(header_size);

	size_t total_recv=0;
	size_t read_bytes=0;
	size_t next_bytes=header_size;
	do{
		read_bytes=m_socket.read_some(boost::asio::buffer(&recv_header_buffer[0],next_bytes));
		if(read_bytes<0)
		{
			return ERR_RECV_PACKET_ERROR;
		}
		else
		{
			next_bytes-=read_bytes;
			total_recv+=read_bytes;
		}
	}while(next_bytes>0);

	char *pheader=&recv_header_buffer[0];
	int hmark=*(int *)pheader;
	if(HEAD_MARK!=hmark)
	{
		return ERR_DECODE_ERROE;		
	}

	istreambuf b(&recv_header_buffer[0],header_size);
	input_stream rd(&b);

	MSG_COMMON_REQ_FIELDS(MSG_COMMON,robj);
	int ret=unserialize_header_packet(rd,robj);
	if(0!=ret)
	{
		return ret;	
	}

	int body_size=robj[FIELD_BSIZE].ivalue;
	int method_send=robj[FIELD_METHOD].ivalue;
	int method_recv=fields[FIELD_METHOD].ivalue;
	
	bool parse_packet=true;
	if(method_send == method_recv)
	{
		copy_fields(fields,robj,true);
		parse_packet=true;	
	}
	else if(MSG_COMMON == method_send)
	{
		ret=robj[FIELD_RETURN].ivalue;

		parse_packet=false;
	}
	else
	{
		ret=ERR_UNEXPECTIED_METHOD;

		parse_packet=false;

		//fields[FIELD_METHOD].ivalue=method_send;
	}

	std::vector<char> recv_body_buffer;
        recv_body_buffer.resize(body_size);
	
	total_recv=0;
        read_bytes=0;
        next_bytes=body_size;
        do{
                read_bytes=m_socket.read_some(boost::asio::buffer(&recv_body_buffer[0],next_bytes));
                if(read_bytes<0)
                {
                        return ERR_RECV_PACKET_ERROR;
                }
                else
                {
                        next_bytes-=read_bytes;
                        total_recv+=read_bytes;
                }
        }while(next_bytes>0);

	if( true==parse_packet )
	{
		char *pbody=&recv_body_buffer[0];
		int bmark=*(int *)pbody;

		if(bmark!=BODY_MARK)
			return ERR_DECODE_ERROE;


		istreambuf b1(&recv_body_buffer[0],header_size);
		input_stream rd1(&b1);	
		ret=unserialize_body_packet(rd1,fields,len);
	}
	
	return ret;
}
