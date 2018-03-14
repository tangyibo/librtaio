#include "common/output_stream.h"

output_stream::output_stream(ostreambuf *b)
:std::ostream(b)
,m_is_little_order(is_little_endian())
{
}

output_stream::~output_stream()
{
}

bool output_stream::write_byte(Byte b)
{
	UChar &x=static_cast<UChar &>(b);
	return this->write_1(&x);
}

bool output_stream::write_char(Char c)
{
	UChar x=static_cast<UChar>(c);
	return this->write_1(&x);
}

bool output_stream::write_uchar(UChar c)
{
	UChar x=static_cast<UChar>(c);
	return this->write_1(&x);	
}

bool output_stream::write_short(Short i)
{
	UShort x=static_cast<UShort>(i);
	return this->write_2(&x);
}

bool output_stream::write_ushort(UShort i)
{
        UShort x=static_cast<UShort>(i);
        return this->write_2(&x);
}

bool output_stream::write_long(Long i)
{
        ULong x=static_cast<ULong>(i);
        return this->write_4(&x);
}

bool output_stream::write_ulong(ULong i)
{
        ULong x=static_cast<ULong>(i);
        return this->write_4(&x);
}

bool output_stream::write_llong(LongLong i)
{
        ULongLong x=static_cast<ULongLong>(i);
        return this->write_8(&x);
}

bool output_stream::write_ullong(ULongLong i)
{
	ULongLong x=static_cast<ULongLong>(i);
        return this->write_8(&x);
}

bool output_stream::write_float(Float i)
{
        ULong x=static_cast<ULong>(i);
        return this->write_4(&x);
}

bool output_stream::write_double(Double i)
{
        ULongLong x=static_cast<ULongLong>(i);
        return this->write_8(&x);
}

bool output_stream::write_byte_array(Byte* x,size_t length)
{
	void *p=static_cast<void *>(x);
	return this->write_array(p,1,length);
}

bool output_stream::write_char_array(Char* x,size_t length)
{
        void *p=static_cast<void *>(x);
        return this->write_array(p,1,length);
}

bool output_stream::write_uchar_array(UChar* x,size_t length)
{
	void *p=static_cast<void *>(x);
        return this->write_array(p,1,length);
}

bool output_stream::write_short_array(Short* x,size_t length)
{
        void *p=static_cast<void *>(x);
        return this->write_array(p,2,length);
}

bool output_stream::write_ushort_array(UShort* x,size_t length)
{
        void *p=static_cast<void *>(x);
        return this->write_array(p,2,length);
}

bool output_stream::write_long_array(Long* x,size_t length)
{
        void *p=static_cast<void *>(x);
        return this->write_array(p,4,length);
}

bool output_stream::write_llong_array(LongLong* x,size_t length)
{
        void *p=static_cast<void *>(x);
        return this->write_array(p,4,length);
}

bool output_stream::write_ullong_array(ULongLong* x,size_t length)
{
        void *p=static_cast<void *>(x);
        return this->write_array(p,8,length);
}

bool output_stream::write_float_array(Float* x,size_t length)
{
        void *p=reinterpret_cast<void *>(x);
        return this->write_array(p,4,length);
}

bool output_stream::write_double_array(Double* x,size_t length)
{
        void *p=reinterpret_cast<void *>(x);
        return this->write_array(p,8,length);
}

bool output_stream::write_1(UChar *x)
{
	char *tmp=reinterpret_cast<char *>(x);
	return this->write(tmp,1);
}

bool output_stream::write_2(UShort *x)
{
	UShort i=*x;
	if(m_is_little_order)
	{
		i=swap_2(i);
	}

	char *tmp=reinterpret_cast<char *>(&i);
	return this->write(tmp,2);
}
 
bool output_stream::write_4(ULong *x)
{
        ULong i=*x;
        if(m_is_little_order)
        {
                i=swap_4(i);
        }

	char *tmp=reinterpret_cast<char *>(&i);
	return this->write(tmp,4);
}

bool output_stream::write_8(ULongLong *x)
{
	ULongLong i=*x;
        if(m_is_little_order)
        {
                i=swap_8(i);
        }

	char *tmp=reinterpret_cast<char *>(&i);
	return this->write(tmp,8);
}

bool output_stream::write_array(void *x,size_t size,size_t count)
{
	size_t ret_count=0;
	switch(size)
	{
	case 1:
	{
		UChar *p1=reinterpret_cast<UChar *>(x);
                for(size_t i=0;i<count;++i)
                {
                        if(write_1(&p1[i])) ++ret_count;
                }
		break;
	}
	case 2:
	{
		UShort *p2=reinterpret_cast<UShort *>(x);
		for(size_t i=0;i<count;++i)
		{
			if(write_2(&p2[i])) ++ret_count;
		}
		break;
	}
	case 4:
	{
                ULong *p4=reinterpret_cast<ULong *>(x);
                for(size_t i=0;i<count;++i)
                {
                        if(write_4(&p4[i])) ++ret_count;
                }
		break;
	}
	case 8:
	{
                ULongLong *p8=reinterpret_cast<ULongLong *>(x);
                for(size_t i=0;i<count;++i)
                {
                        if(write_8(&p8[i])) ++ret_count;
                }
		break;
	}
	default:
		break;
	}

	return (ret_count==count);
}
