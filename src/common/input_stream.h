#ifndef __INPUT_STREAM_HEADER_H_
#define __INPUT_STREAM_HEADER_H_
#include <istream>
#include "common/net_stream.h"
#include "common/io_streambuf.h"

class input_stream:public std::istream
{
private:
	bool m_is_little_order;

public:
	input_stream(istreambuf *b);
	virtual ~input_stream();

	char *base_ptr() const 
	{ 
		istreambuf *buf=dynamic_cast<istreambuf *>(rdbuf());
		return buf->base_ptr(); 
	}

	char *read_ptr() const 
	{ 
		istreambuf *buf=dynamic_cast<istreambuf *>(rdbuf());
		return buf->read_ptr(); 
	}

	char *end_ptr() const 
	{
		istreambuf *buf=dynamic_cast<istreambuf *>(rdbuf()); 
		return buf->end_ptr(); 
	}

	bool read_byte(Byte &b);
	bool read_char(Char &c);
	bool read_uchar(UChar &c);
	bool read_short(Short &i);
	bool read_ushort(UShort &i);
	bool read_long(Long &i);
	bool read_ulong(ULong &i);
	bool read_llong(LongLong &i);
	bool read_ullong(ULongLong &i);
	bool read_float(Float &i);
	bool read_double(Double &i);

	bool read_byte_array(Byte* x,size_t length);
	bool read_char_array(Char* x,size_t length);
	bool read_uchar_array(UChar* x,size_t length);
	bool read_short_array(Short* x,size_t length);
	bool read_ushort_array(UShort* x,size_t length);
	bool read_long_array(Long* x,size_t length);
	bool read_llong_array(LongLong* x,size_t length);
	bool read_ullong_array(ULongLong* x,size_t length);
	bool read_float_array(Float* x,size_t length);
	bool read_double_array(Double* x,size_t length);

protected:
	bool read_1(UChar *x);
	bool read_2(UShort *x);
	bool read_4(ULong *x);
	bool read_8(ULongLong *x);
	bool read_array(void *x,size_t size,size_t count);
};
#endif
