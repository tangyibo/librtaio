#ifndef __ASYNC_ACCEPTOR_HEADER_H__
#define __ASYNC_ACCEPTOR_HEADER_H__
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <set>

//#define ACCEPT_CONNECT_CLIENT_COUNT  100

class session_handler;
class async_acceptor
{
public:
        async_acceptor(boost::asio::io_service& io_service, short port);
        virtual ~async_acceptor();

        void start();

        void handle_accept(session_handler* handler,const boost::system::error_code& error);

	session_handler* make_session_handler();
	void remove_session_handler(session_handler* handler);

private:
        boost::asio::io_service& m_io_service;
        boost::asio::ip::tcp::acceptor m_acceptor;

	typedef std::set< session_handler* > SESSION_LIST;
	boost::mutex m_mtx_handlers;	
	SESSION_LIST m_set_handlers;
};

#endif
