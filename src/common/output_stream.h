#ifndef __OUTPUT_STREAM_HEADER_H_
#define __OUTPUT_STREAM_HEADER_H_
#include <ostream>
#include "common/net_stream.h"
#include "common/io_streambuf.h"

class output_stream:public std::ostream
{
private:
	bool m_is_little_order;

public:
	output_stream(ostreambuf *b);
	virtual ~output_stream();

	char *base_ptr() const 
	{ 
		ostreambuf *buf=(dynamic_cast<ostreambuf *>(rdbuf()));
		return buf->base_ptr();
	}

	char *write_ptr() const 
	{ 
		ostreambuf *buf=(dynamic_cast<ostreambuf *>(rdbuf()));
		return buf->write_ptr();
	}
	
	char *end_ptr() const 
	{ 
		ostreambuf *buf=(dynamic_cast<ostreambuf *>(rdbuf()));
		return buf->end_ptr(); 
	}

	bool write_byte(Byte b);
	bool write_char(Char c);
	bool write_uchar(UChar c);
	bool write_short(Short i);
	bool write_ushort(UShort i);
	bool write_long(Long i);
	bool write_ulong(ULong i);
	bool write_llong(LongLong i);
	bool write_ullong(ULongLong i);
	bool write_float(Float i);
	bool write_double(Double i);

	bool write_byte_array(Byte* x,size_t length);
	bool write_char_array(Char* x,size_t length);
	bool write_uchar_array(UChar* x,size_t length);
	bool write_short_array(Short* x,size_t length);
	bool write_ushort_array(UShort* x,size_t length);
	bool write_long_array(Long* x,size_t length);
	bool write_llong_array(LongLong* x,size_t length);
	bool write_ullong_array(ULongLong* x,size_t length);
	bool write_float_array(Float* x,size_t length);
	bool write_double_array(Double* x,size_t length);

protected:
	bool write_1(UChar *x);
	bool write_2(UShort *x);
	bool write_4(ULong *x);
	bool write_8(ULongLong *x);
	bool write_array(void *x,size_t size,size_t count);
};
#endif
