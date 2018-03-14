#include "common/net_stream.h"
#include <string.h>

uint16_t swap_2(uint16_t v)
{
	unsigned char arr[2];
	memcpy(arr,&v,2);
	unsigned char byte;
	
	byte = arr[0];
	arr[0] = arr[1];
	arr[1] = byte;
	return *(uint16_t*)arr;	
}

uint32_t swap_4(uint32_t v)
{
    unsigned char arr[4];
    memcpy(arr,&v,4);
    unsigned char byte;

    byte = arr[0];
    arr[0] = arr[3];
    arr[3] = byte;

    byte = arr[1];
    arr[1] = arr[2];
    arr[2] = byte;

    return *(uint32_t*)arr;
}

uint64_t swap_8(uint64_t v)
{
        uint32_t low = (uint32_t) (v & 0x00000000FFFFFFFFLL);
        uint32_t high = (uint32_t) ((v & 0xFFFFFFFF00000000LL) >> 32);
        
        low = swap_4(low);
        high = swap_4(high);

        return  (uint64_t) high + (((uint64_t) low) << 32);
}

bool is_little_endian()
{
	//判断字节序
	int temp=1;
	unsigned char byte_order=((unsigned char *)&temp)[0];
	return (byte_order==1);
}
