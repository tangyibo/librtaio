#include "core/network/async_acceptor.h"
#include <cstdlib>
#include <iostream>
#include <boost/functional.hpp>
#include <boost/typeof/typeof.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include "core/network/session_handler.h"
#include "protocol/packet_protocol.h"
#define LOG4Z_FORMAT_INPUT_ENABLE
#include "libs/log4z.h"

///////////////////////////////////////////////////////////////////////
async_acceptor::async_acceptor(boost::asio::io_service& io_service, short port)
: m_io_service(io_service)
, m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
}

async_acceptor::~async_acceptor()
{
}

void async_acceptor::start()
{
        boost::asio::ip::tcp::endpoint ep=m_acceptor.local_endpoint();
        LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID,"Server listen on %s:%d",ep.address().to_string().c_str(),ep.port());

        session_handler* handler = make_session_handler();
        m_acceptor.async_accept(handler->socket(),
                        boost::bind(&async_acceptor::handle_accept, this, handler,
                                boost::asio::placeholders::error));

}

void async_acceptor::handle_accept(session_handler* handler,const boost::system::error_code& error)
{
        if (!error)
        {
                handler->start();

                handler = make_session_handler();
                m_acceptor.async_accept(handler->socket(),
                                boost::bind(&async_acceptor::handle_accept, this, handler,
                                        boost::asio::placeholders::error));
        }
        else
        {
                remove_session_handler(handler);
		delete handler;
        }
}

session_handler* async_acceptor::make_session_handler()
{
	session_handler* handler=new (std::nothrow)session_handler(m_io_service,this);	
	if(handler)
	{
		boost::mutex::scoped_lock locker(m_mtx_handlers);
		m_set_handlers.insert(handler);
		
		return handler;
	}

	return NULL;
}

void async_acceptor::remove_session_handler(session_handler* handler)
{
	if(handler)
	{
		boost::mutex::scoped_lock locker(m_mtx_handlers);
                m_set_handlers.erase(handler);
	}		
}
