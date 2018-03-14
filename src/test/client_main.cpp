#include "api/rtdb_client.h"
#include <iostream>

int main(int argc,char *argv[])
{
	rtdb_client cli("127.0.0.1",7654);
	
	int ret=cli.connect();
	if(0==ret)
	{
		std::cout<<"Connect server OK...!"<<std::endl;

		//
		//other operations here!
		//

		cli.disconnect();
	}

	return 0;
}
