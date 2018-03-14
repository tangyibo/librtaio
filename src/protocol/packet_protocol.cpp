#include "protocol/packet_protocol.h"
#include <boost/crc.hpp> // for boost::crc_basic, boost::crc_optimal
#include "include/error_code.h"

static uint32_t crc32( const void* data, size_t size )  
{  
    boost::crc_32_type crc32_obj;  
    crc32_obj.process_bytes(data, size);  
    return crc32_obj();  
}  

int serialize_packet(output_stream &out,FIELD *fields,int len)
{
	char *body_base=0;
	int n=0;

	for(FIELD *p=fields; p->type >=0 ; ++p,++n)
	{
		/*Caculate CRC for header crc field*/
		if(n==FIELD_HCRC)
		{
			unsigned int length=out.write_ptr()-out.base_ptr();
			if(length>0)
			{
				p->ivalue=crc32(out.base_ptr(),length);
			}
		}
		/*Caculate CRC for body crc field*/
		else if( len-1==n )
		{
			if(0!=body_base)
			{
				int length=out.write_ptr()-body_base;
				if(length>0)
					p->ivalue=crc32(body_base,length);
			}
		}

		/**************caculate binary stream************************/
		if(0==p->data)
		{
			bool ret=false;
			switch(p->type)
			{
			case TYPE_CHAR:
			case TYPE_BYTE:
				ret=out.write_char((Char)p->ivalue);
				break;
			case TYPE_SHORT:
				ret=out.write_short((Short)p->ivalue);
				break;
			case TYPE_LONG:
				ret=out.write_long((Long)p->ivalue);
				break;
			case TYPE_LONGLONG:
				ret=out.write_llong((LongLong )p->ivalue);
				break;
			case TYPE_FLOAT:
				ret=out.write_float((Float)p->fvalue);
				break;
			case TYPE_DOUBLE:
				ret=out.write_double((Double)p->fvalue);
				break;
			default:
				break;
			}
		}
		else /*p->data !=0*/
		{
			if(p->count >0 )/*p->data !=0 && p->count >0*/
			{
				int count=p->count * fields[FIELD_COUNT].ivalue;
				switch(p->type)
				{
				case TYPE_CHAR:
				case TYPE_BYTE:
					out.write_char_array((Char *)p->data,count);
					break;
				case TYPE_SHORT:
					out.write_short_array((Short *)p->data,count);
					break;
				case TYPE_LONG:
					out.write_long_array((Long*)p->data,count);
					break;
				case TYPE_LONGLONG:
					out.write_llong_array((LongLong*)p->data,count);
					break;
				case TYPE_FLOAT:
					out.write_float_array((Float*)p->data,count);
					break;
				case TYPE_DOUBLE:
					out.write_double_array((Double*)p->data,count);
					break;
				default:
					break;
				}
			}
			else /*p->data !=0 && p->count==0*/
			{
				FIELD *prev=p-1;
				if(prev->type!=TYPE_SHORT && prev->type!=TYPE_LONG)
					return ERR_ENCODE_ERROE;

				int count=0;
				switch(p->count)
				{
				case -1:
					{
						count=prev->ivalue;
						switch(p->type)
						{
						case TYPE_CHAR:
						case TYPE_BYTE:
							out.write_char_array((Char *)p->data,count);
							break;
						case TYPE_SHORT:
							out.write_short_array((Short *)p->data,count);
							break;
						case TYPE_LONG:
							out.write_long_array((Long*)p->data,count);
							break;
						case TYPE_LONGLONG:
							out.write_llong_array((LongLong*)p->data,count);
							break;
						case TYPE_FLOAT:
							out.write_float_array((Float*)p->data,count);
							break;
						case TYPE_DOUBLE:
							out.write_double_array((Double*)p->data,count);
							break;
						default:
							break;
						}
						break;
					}
				case -2://reserved!
					break;
				case -3://reserved!
					break;
				case -4://reserved!
					break;
				default:
					break;
				}
			}
		}

		if(n==FIELD_BMARK -1)
			body_base=out.write_ptr();

		if(!out.good())
			return ERR_ENCODE_ERROE;
	}

	return ERR_OK;
}


int unserialize_header_packet(input_stream &in,FIELD *fields, bool compare_crc)
{
	Char c_val=0;
	Short s_val=0;
	Long l_val=0;
	LongLong ll_val=0;
	Float f_val=0.;
	Double d_val=0;
	int n=0;
	unsigned int crc=0;
	char *base=in.base_ptr();

	for(FIELD *p=fields; p->type >=0 ; ++p,++n)
	{
		if(n==FIELD_HCRC)
		{
			int length=in.read_ptr() - base;
			if(length>0 && compare_crc)
				crc=crc32(base,length);
		}

		if(p->data == 0)
		{
			switch(p->type)
			{
			case TYPE_CHAR:
			case TYPE_BYTE:
				in.read_char(c_val);
				p->ivalue=c_val;
				break;
			case TYPE_SHORT:
				in.read_short(s_val);
				p->ivalue=s_val;
				break;
			case TYPE_LONG:
				in.read_long(l_val);
				p->ivalue=l_val;
				break;
			case TYPE_LONGLONG:
				in.read_llong(ll_val);
				p->ivalue=ll_val;
				break;
			case TYPE_FLOAT:
				in.read_float(f_val);
				p->fvalue=f_val;
				break;
			case TYPE_DOUBLE:
				in.read_double(d_val);
				p->fvalue=d_val;
				break;
			default:
				break;
			}
		}

		if(n==FIELD_HMARK)
		{
			if((unsigned int)p->ivalue!=HEAD_MARK)
				return ERR_DECODE_ERROE;
		}
		else if(n==FIELD_HCRC)
		{
			if(compare_crc && crc!=(unsigned int)p->ivalue)
				return ERR_CHECK_CRC_ERROR;

			break;
		}

		if(!in.good())
			return ERR_DECODE_ERROE;
	}

	return ERR_OK;
}

int unserialize_body_packet(input_stream &in,FIELD *fields, int len,bool compare_crc)
{
	Char c_val=0;
	Short s_val=0;
	Long l_val=0;
	LongLong ll_val=0;
	Float f_val=0.;
	Double d_val=0;

	int n=FIELD_BMARK;
	unsigned int crc=0;
	int cnt=fields[FIELD_COUNT].ivalue;
	char *base=in.read_ptr();

	for(FIELD *p=&fields[n]; p->type >=0 ; ++p,++n)
	{
		if(len-1==n && compare_crc)
		{
			int length=in.read_ptr()-base;
			if(length>0)
				crc=crc32(base,length);
		}

		if(p->data==0)
		{
			bool ret=false;
			switch(p->type)
			{
			case TYPE_CHAR:
			case TYPE_BYTE:
				ret=in.read_char(c_val);
				p->ivalue=c_val;
				break;
			case TYPE_SHORT:
				ret=in.read_short(s_val);
				p->ivalue=s_val;
				break;
			case TYPE_LONG:
				ret=in.read_long(l_val);
				p->ivalue=l_val;
				break;
			case TYPE_LONGLONG:
				ret=in.read_llong(ll_val);
				p->ivalue=ll_val;
				break;
			case TYPE_FLOAT:
				ret=in.read_float(f_val);
				p->fvalue=f_val;
				break;
			case TYPE_DOUBLE:
				ret=in.read_double(d_val);
				p->fvalue=d_val;
				break;
			default:
				break;
			}

		}
		else // p->data != 0
		{
			int count=0;
			if(p->count >=0) // p->data != 0  && p->count >= 0
			{
				count=cnt * p->count;
				switch(p->type )
				{
				case TYPE_CHAR:
				case TYPE_BYTE:
					in.read_char_array((Char *)p->data,count);
					break;
				case TYPE_SHORT:
					in.read_short_array((Short *)p->data,count);
					break;
				case TYPE_LONG:
					in.read_long_array((Long *)p->data,count);
					break;
				case TYPE_LONGLONG:
					in.read_llong_array((LongLong *)p->data,count);
					break;
				case TYPE_FLOAT:
					in.read_float_array((Float *)p->data,count);
					break;
				case TYPE_DOUBLE:
					in.read_double_array((Double *)p->data,count);
					break;
				default:
					break;
				}
			}
			else// p->data != 0  && p->count < 0
			{
				FIELD *prev=p-1;
				if(prev->type!=TYPE_SHORT && prev->type !=TYPE_LONG)
					return ERR_DECODE_ERROE;

				switch(p->count)
				{
				case -1:
					{
						if(prev->data != 0)
							return ERR_DECODE_ERROE;

						count=prev->ivalue;
						if(count >0)
						{
							switch(p->type )
							{
							case TYPE_CHAR:
							case TYPE_BYTE:
								in.read_char_array((Char *)p->data,count);
								break;
							case TYPE_SHORT:
								in.read_short_array((Short *)p->data,count);
								break;
							case TYPE_LONG:
								in.read_long_array((Long *)p->data,count);
								break;
							case TYPE_LONGLONG:
								in.read_llong_array((LongLong *)p->data,count);
								break;
							case TYPE_FLOAT:
								in.read_float_array((Float *)p->data,count);
								break;
							case TYPE_DOUBLE:
								in.read_double_array((Double *)p->data,count);
								break;
							default:
								break;
							}
						}
					}
				case -2://reserved!
					break;
				case -3://reserved!
					break;
				case -4://reserved!
					break;
				default:
					break;
				}
			}
		}

		if(n==len-1)
		{
			if(compare_crc && (unsigned int)p->ivalue !=crc)
				return ERR_CHECK_CRC_ERROR;
		}
	}

	return ERR_OK;

}

int copy_fields(FIELD *dest,FIELD *src, bool only_header/*=false*/)
{
	int n=0;
	for(FIELD *p=src; p->type >0 ; ++p,++n)
	{
		dest[n]=*p;
		if(only_header  &&  n==FIELD_BMARK-1 )
			break;
	}

	if(!only_header)
		dest[n].type=-1;

	return n;
}


void fields_size(FIELD *fields,int *total,int *header,int *body,int *body_count)
{
	if(total) *total=0;
	if(header) *header=0;
	if(body) *body=0;

	int n=0;
	int cnt=fields[FIELD_COUNT].ivalue;

	for(FIELD *p=fields; p->type >= 0 ; ++p,++n)
	{
		int size=0;
		switch(p->type)
		{
		case TYPE_CHAR:
		case TYPE_BYTE:
			size=1;
			break;
		case TYPE_SHORT:
			size=2;
			break;
		case TYPE_LONG:
			size=4;
			break;
		case TYPE_LONGLONG:
			size=8;
			break;
		case TYPE_FLOAT:
			size=4;
			break;
		case TYPE_DOUBLE:
			size=8;
			break;
		default:
			break;
		}

		if(p->data !=0)
		{
			if(p->count >=0)
			{
				size *=p->count * cnt;
			}
			else
			{
				//reserved!
				switch(p->count)
				{
				case -1:
					{
						FIELD *prev=p-1;
						if(prev->data == 0)
							size *=prev->ivalue;
						break;
					}
				case -2:
					break;
				case -3:
					break;
				case -4:
					break;
				default:
					break;
				}
			}
		}

		if(n==FIELD_NEXT)
		{
			if(body_count)
				*body_count=n;
		}

		if(total)
		{
			*total+=size;
		}

		if(n<=FIELD_HCRC)
		{
			if(header)
				*header+=size;
		}
		else
		{
			if(body)
				*body+=size;
		}
	}

	if(body_count)
		*body_count=n-*body_count+1;
}
