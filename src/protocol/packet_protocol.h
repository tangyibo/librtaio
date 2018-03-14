#ifndef _PACKET_PROTOCOL_HEADER_H_
#define _PACKET_PROTOCOL_HEADER_H_
#include "include/error_code.h"
#include "common/input_stream.h"
#include "common/output_stream.h"

#define  HEAD_MARK	0x11111111
#define  BODY_MARK	0x22222222

#define FIELDS_LEN(fields)	 (sizeof(fields) / sizeof(FIELD) -1)

typedef struct _field_
{
	int type;
	long long ivalue;
	double fvalue;
	const void *data;
	int count;
	const char *name;
}FIELD;

enum MSG_DATA_TYPE
{
	TYPE_CHAR=0,  //char
	TYPE_BYTE,    //byte
	TYPE_SHORT,   //short
	TYPE_LONG,    //long
	TYPE_LONGLONG,//long long
	TYPE_FLOAT,   //float
	TYPE_DOUBLE   //double
};

enum MSG_FIELD_NUMBER
{
	FIELD_HMARK=0, //header mark
	FIELD_METHOD,  //method id
	FIELD_BSIZE,   //body size
	FIELD_RETURN,  //return value
	FIELD_TIME,    //time stamp
	FIELD_COUNT,   //count
	FIELD_HCRC,    //header crc
	FIELD_BMARK,   //body mark
	FIELD_NEXT     //...
};

enum MSG_ID
{
	MSG_NULL=0,
	MSG_COMMON,

	/**Connection Messages**/
	MSG_CONNECT,
	MSG_DISCONNECT,
	MSG_GET_CONNECT_COUNT,
	MSG_GET_CONNECT_INFO,
	MSG_GET_CONNECTIONS,
	MSG_KILL_CONNECTION,
	MSG_SET_TIMEOUT,
	MSG_GET_TIMEOUT,

	/**User Messages**/
	MSG_USER_LOGIN,
	MSG_USER_LOGOUT,
	MSG_ADD_USER,
	MSG_CHANGE_PWD,
	MSG_CHANGE_PRIVILAGE,
	MSG_GET_USER_COUNT,
	MSG_GET_USERS,
	MSG_DELETE_USER,
};

#define MSG_HEADER(method,ret,count)	\
{TYPE_LONG, HEAD_MARK, 0.0, 0,1,"hmark"},\
{TYPE_LONG, method, 0.0 , 0, 1, "method"},\
{TYPE_LONG, 0, 0.0, 0, 1, "bsize"},\
{TYPE_LONG, ret, 0.0 , 0, 1, "ret"},\
{TYPE_LONG, time(NULL), 0.0 , 0, 1, "time"},\
{TYPE_LONG, count,0.0 , 0, 1, "count"},\
{TYPE_LONGLONG, 0, 0.0 , 0, 1, "hcrc"},/*will be computed later*/\
{TYPE_LONG, BODY_MARK, 0.0, 0,1,"bmark"},	\

#define MSG_TAIL()\
{TYPE_LONGLONG, 0, 0.0, 0, 1,"bcrc"},/*will be computed later*/\
{-1,0,0.0,0,0,""},\

#define MSG_COMMON_REQ_FIELDS(method,obj)	\
FIELD obj[]=\
{\
	MSG_HEADER(method, 0, 1)\
	MSG_TAIL()	\
}

#define MSG_COMMON_RESP_FIELDS(method,ret,obj)\
	FIELD obj[]=\
{\
	MSG_HEADER(method, ret, 1)\
	MSG_TAIL()	\
}


int serialize_packet(output_stream &out,FIELD *fields,int len);
int unserialize_header_packet(input_stream &in,FIELD *fields, bool compare_crc=true);
int unserialize_body_packet(input_stream &in,FIELD *fields, int len,bool compare_crc=true);

int copy_fields(FIELD *dest,FIELD *src, bool only_header=false);
void fields_size(FIELD *fields,int *total,int *header,int *body,int *body_count);

#endif
