#ifndef _RTDB_CLIENT_HEADER_H_
#define _RTDB_CLIENT_HEADER_H_
#include <boost/smart_ptr.hpp>
#include <boost/thread/mutex.hpp>

class rtdb_client
{
public:
	class rtdb_client_impl;

public:
	rtdb_client(const char *ipaddr,const int port);
	~rtdb_client();

	int connect();
	void disconnect();

private:
	rtdb_client(const rtdb_client &);
	void operator=(const rtdb_client &);

	boost::shared_ptr<rtdb_client::rtdb_client_impl> m_ptr_impl;
	boost::mutex m_mutex_lock;
};

#endif
