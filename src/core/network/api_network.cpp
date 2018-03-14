#include "core/network/session_handler.h"
#include "protocol/protocol_network.h"
#define LOG4Z_FORMAT_INPUT_ENABLE
#include "libs/log4z.h"

int session_handler::msg_connect()
{
	int ret=0;
	char clientname[1024]={0};
	char clientprocess[1024]={0};

	{
		MSG_CONNECT_FIELDS(clientname,clientprocess,robj);
		ret=read_packet_body(robj,FIELDS_LEN(robj));
	}

	if(ret!=ERR_OK)
		return ret;

	LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID,"Client hostname:[%s],process[%s] is connected!",clientname,clientprocess);

	MSG_CONNECT_RESP_FIELDS(ERR_OK,true,wobj);
	return send_packet(wobj,FIELDS_LEN(wobj));
}

int session_handler::msg_disconnect()
{
	int ret=0;

	{
		MSG_DISCONNECT_FIELDS(robj);
		ret=read_packet_body(robj,FIELDS_LEN(robj));
	}

	if(ret!=ERR_OK)
		return ret;

	LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID,"Client Request disconnect now!");
	close();
	
        return 0;
}

