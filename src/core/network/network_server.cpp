#include "core/network/network_server.h"
#include "core/network/async_acceptor.h"
#define LOG4Z_FORMAT_INPUT_ENABLE
#include "libs/log4z.h"
#include <iostream>

using namespace zsummer::log4z;

network_server::network_server(const int port,const int conn_num)
:m_net_acceptor(m_net_server,port)
{
}

network_server::~network_server()
{
}


void network_server::run()
{
	try
	{
		LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID,"Start module [network] now ...");

                m_net_acceptor.start();
		std::cout<<"Server start OK!"<<std::endl;

		m_net_server.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
		LOGFMT_ERROR(LOG4Z_MAIN_LOGGER_ID,"Exception: %s " ,e.what());
	}
}

void network_server::stop()
{
	m_net_server.stop();
}
