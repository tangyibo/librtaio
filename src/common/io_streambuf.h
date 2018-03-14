#ifndef _IO_STREAM_HEADER_H
#define _IO_STREAM_HEADER_H
#include <streambuf>
#include <stdio.h>
#include <assert.h>

class istreambuf : public std::streambuf 
{
public:
	istreambuf(char *base,const size_t len):std::streambuf()
	{
		this->setbuf(base,len);
		this->setg(base,base,base+len);//todo: here is setg(set get position)		
	}

	//override function
	virtual int_type underflow()
        {
		assert(0);
                //printf("buffer underflow for read charactor \n");
                return EOF;
        }

	char *base_ptr() const { return this->eback(); }
	char *read_ptr() const { return this->gptr(); }
	char *end_ptr() const { return this->egptr(); }

};


class ostreambuf  : public std::streambuf
{
public:
	ostreambuf(char *base,const size_t len):std::streambuf()
	{
		this->setbuf(base,len);
		this->setp(base,base+len);//todo: here is setp(set put position)
	}

	//override function
	virtual int_type overflow(int_type ch)
	{
		assert(0);
		//printf("buffer overflow for write charactor %c\n",ch);
		return ch;
	}

        char *base_ptr() const { return this->pbase(); }
        char *write_ptr() const { return this->pptr(); }
        char *end_ptr() const { return this->epptr(); }

};

#endif
