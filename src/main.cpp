#include "libs/app.h"
#define LOG4Z_FORMAT_INPUT_ENABLE
#include "libs/log4z.h"
#include "core/network/network_server.h"
#include <boost/thread.hpp>
#include <boost/functional.hpp>
#include <boost/bind.hpp>

#define APP_NAME "rtdb-server"
#define APP_VERSION "v1.0.1"

extern volatile bool g_quit;

class MyApp:public Application
{
public:
	virtual void usage(int argc, char **argv)
	{
		printf("Usage:\n");
		printf("    %s [-d] /path/to/config.ini [-s start|stop|restart]\n", argv[0]);
		printf("Options:\n");
		printf("    -d    run as daemon\n");
		printf("    -s    option to start|stop|restart the server\n");
		printf("    -h    show this message\n");
	}

	virtual void welcome()
	{
		fprintf(stderr, "%s %s\n", APP_NAME, APP_VERSION);
                fprintf(stderr, "Copyright (c) 2018-2019 tang\n");
                fprintf(stderr, "\n");
	}

	virtual void run()
	{
		LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID,"Start server ...");

		int server_port=7654;
		int connet_num=100;
		network_server srv(server_port,connet_num);

		boost::function<void ()> fun=boost::bind(&network_server::run,&srv);
		boost::thread t(fun);		

		while(!g_quit)
		{
			sleep(10);
		}

		srv.stop();
		t.join();
		
		LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID,"Stop server ...");
	}
};

int main(int argc,char *argv[])
{
	MyApp app;	
	return app.main(argc,argv);
}
