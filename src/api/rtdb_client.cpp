#include "api/rtdb_client.h"
#include "api/rtdb_client_impl.h"

rtdb_client::rtdb_client(const char *ipaddr,const int port)
:m_ptr_impl(new rtdb_client::rtdb_client_impl(ipaddr,port))
,m_mutex_lock()
{
}

rtdb_client::~rtdb_client()
{
	boost::mutex::scoped_lock locker(m_mutex_lock);
	m_ptr_impl.reset();
}

int rtdb_client::connect()
{
	boost::mutex::scoped_lock locker(m_mutex_lock);
	return m_ptr_impl->connect();	
}

void rtdb_client::disconnect()
{
	boost::mutex::scoped_lock locker(m_mutex_lock);
	m_ptr_impl->disconnect();
}
