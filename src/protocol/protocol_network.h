#ifndef _PROTOCOL_NETWORK_HEADER_H_
#define _PROTOCOL_NETWORK_HEADER_H_
#include "protocol/packet_protocol.h"

#define MSG_CONNECT_FIELDS(clientname,processname,obj)\
FIELD obj[]=\
{\
	MSG_HEADER(MSG_CONNECT,0,1)\
	{TYPE_SHORT, strlen(clientname)+1, 0.0 , 0, 1, "clientname_len"},\
	{TYPE_CHAR, 0, 0.0 , clientname, -1, "clientname"},\
	{TYPE_SHORT, strlen(processname)+1, 0.0 , 0, 1, "processname_len"},\
	{TYPE_CHAR, 0, 0.0 , processname, -1, "processname"},\
	MSG_TAIL()\
}

#define MSG_CONNECT_RESP_FIELDS(retcode,priv ,obj)\
enum{FIELD_PRIV=FIELD_NEXT};\
FIELD obj[]=\
{\
	MSG_HEADER(MSG_CONNECT, retcode, 1)\
	{TYPE_SHORT, priv, 0.0 , 0, 1, "permission"},\
	MSG_TAIL()\
}

#define MSG_DISCONNECT_FIELDS(obj)\
FIELD obj[]=\
{\
	MSG_HEADER(MSG_DISCONNECT,0,1)\
	MSG_TAIL()\
}

#endif
