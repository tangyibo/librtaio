#ifndef _ASIO_HANDLER_HEADER_H_
#define _ASIO_HANDLER_HEADER_H_
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <vector>
#include "protocol/packet_protocol.h"

class async_acceptor;
class session_handler
{
public:
	session_handler(boost::asio::io_service& io_service,async_acceptor *acceptor);

	~session_handler();

	inline boost::asio::ip::tcp::socket& socket() {	return m_socket;}

	void start();

	void close();

	void handle_read(const boost::system::error_code& error,size_t bytes_transferred,size_t want_bytes);

	void handle_write(const boost::system::error_code& error,size_t bytes_transferred);

protected:
	int handle_packet_header();
	int read_packet_header();

	int handle_packet_body();
	int read_packet_body(FIELD *fields,int len);

	int send_packet(FIELD *fields,int len);

	//api function all delare follows
	int msg_connect();
	int msg_disconnect();

private:
	boost::asio::ip::tcp::socket m_socket;
	async_acceptor *m_acceptor;

	int m_header_size;
	FIELD m_packet_header[FIELD_BMARK+3];

	std::vector<char> m_recv_buffer;
	std::vector<char> m_send_buffer;
};
#endif
