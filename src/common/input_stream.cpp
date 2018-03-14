#include "common/input_stream.h"

input_stream::input_stream(istreambuf *b)
:std::istream(b)
,m_is_little_order(is_little_endian())
{
}

input_stream::~input_stream()
{
}

bool input_stream::read_byte(Byte &b)
{
	UChar &x=reinterpret_cast<UChar &>(b);
	return this->read_1(&x);
}

bool input_stream::read_char(Char &c)
{
	UChar &x=reinterpret_cast<UChar &>(c);
	return this->read_1(&x);
}

bool input_stream::read_uchar(UChar &c)
{
	UChar &x=reinterpret_cast<UChar &>(c);
	return this->read_1(&x);	
}

bool input_stream::read_short(Short &i)
{
	UShort &x=reinterpret_cast<UShort &>(i);
	return  this->read_2(&x);
}

bool input_stream::read_ushort(UShort &i)
{
	UShort &x=reinterpret_cast<UShort &>(i);
        return this->read_2(&x);
}

bool input_stream::read_long(Long &i)
{
        ULong &x=reinterpret_cast<ULong &>(i);
        return this->read_4(&x);
}

bool input_stream::read_ulong(ULong &i)
{
	ULong &x=reinterpret_cast<ULong &>(i);
        return this->read_4(&x);
}

bool input_stream::read_llong(LongLong &i)
{
        ULongLong &x=reinterpret_cast<ULongLong &>(i);
        return this->read_8(&x);
}

bool input_stream::read_ullong(ULongLong &i)
{
        ULongLong &x=reinterpret_cast<ULongLong &>(i);
        return this->read_8(&x);
}

bool input_stream::read_float(Float &i)
{
        ULong &x=reinterpret_cast<ULong &>(i);
        return this->read_4(&x);
}

bool input_stream::read_double(Double &i)
{
        ULongLong &x=reinterpret_cast<ULongLong &>(i);
        return this->read_8(&x);
}

bool input_stream::read_byte_array(Byte* x,size_t length)
{
	void *p=reinterpret_cast<void *>(x);
	return this->read_array(p,1,length);
}

bool input_stream::read_char_array(Char* x,size_t length)
{
        void *p=reinterpret_cast<void *>(x);
        return this->read_array(p,1,length);
}

bool input_stream::read_uchar_array(UChar* x,size_t length)
{
	void *p=reinterpret_cast<void *>(x);
        return this->read_array(p,1,length);
}

bool input_stream::read_short_array(Short* x,size_t length)
{
        void *p=reinterpret_cast<void *>(x);
        return this->read_array(p,2,length);
}

bool input_stream::read_ushort_array(UShort* x,size_t length)
{
        void *p=reinterpret_cast<void *>(x);
        return this->read_array(p,2,length);
}

bool input_stream::read_long_array(Long* x,size_t length)
{
        void *p=reinterpret_cast<void *>(x);
        return this->read_array(p,4,length);
}

bool input_stream::read_llong_array(LongLong* x,size_t length)
{
        void *p=reinterpret_cast<void *>(x);
        return this->read_array(p,4,length);
}

bool input_stream::read_ullong_array(ULongLong* x,size_t length)
{
        void *p=reinterpret_cast<void *>(x);
        return this->read_array(p,8,length);
}

bool input_stream::read_float_array(Float* x,size_t length)
{
        void *p=reinterpret_cast<void *>(x);
        return this->read_array(p,4,length);
}

bool input_stream::read_double_array(Double* x,size_t length)
{
        void *p=reinterpret_cast<void *>(x);
        return this->read_array(p,8,length);
}

bool input_stream::read_1(UChar *x)
{
	char *tmp=reinterpret_cast<char *>(x);
	return this->read(tmp,1);
}

bool input_stream::read_2(UShort *x)
{
	char *tmp=reinterpret_cast<char *>(x);
	bool ret= this->read(tmp,2);
	if(ret && m_is_little_order)
	{
		UShort val=*reinterpret_cast<UShort *>(tmp);
		*x=swap_2(val);		
	}
	return ret;
}
 
bool input_stream::read_4(ULong *x)
{
	char *tmp=reinterpret_cast<char *>(x);
	bool ret= this->read(tmp,4);
        if(ret && m_is_little_order)
        {       
                ULong val=*reinterpret_cast<ULong *>(tmp);
                *x=swap_4(val);
        }
        return ret;
}

bool input_stream::read_8(ULongLong *x)
{
	char *tmp=reinterpret_cast<char *>(x);
	bool ret= this->read(tmp,8);
        if(ret && m_is_little_order)
        {
                ULongLong val=*reinterpret_cast<ULongLong *>(tmp);
                *x=swap_8(val);
        }
        return ret;

}

bool input_stream::read_array(void *x,size_t size,size_t count)
{
        char *tmp=static_cast<char *>(x);
	size_t bytes=size*count;
        bool ret= this->read(tmp,bytes);
	if(ret && m_is_little_order)
	{
		switch(size)
		{
		case 1:
		{
			break;
		}
		case 2:
		{
			UShort *p=reinterpret_cast<UShort *>(tmp);
			for(size_t i=0;i<count;++i)
			{
				p[i]=swap_2(p[i]);	
			}
			break;
		}
		case 4:
		{
                        ULong *p=reinterpret_cast<ULong *>(tmp);
                        for(size_t i=0;i<count;++i)
                        {
                                p[i]=swap_4(p[i]);
                        }
			break;
		}
		case 8:
		{
                        ULongLong *p=reinterpret_cast<ULongLong *>(tmp);
                        for(size_t i=0;i<count;++i)
                        {
                                p[i]=swap_8(p[i]);
                        }
			break;
		}
		default:
			break;
		};
	}

	return ret;
}
