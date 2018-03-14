#ifndef __NETWORK_SERVER_HEADER_H_
#define __NETWORK_SERVER_HEADER_H_
#include "core/network/async_acceptor.h"

class network_server
{
public:
	network_server(const int port,const int conn_num);
	~network_server();

	void run();
	void stop();
	
protected:
	network_server(const network_server &);

private:
        boost::asio::io_service m_net_server;
	async_acceptor m_net_acceptor;
};

#endif
