#include "libs/app.h"
#include "libs/file.h"
#include "libs/config.h"
#include "libs/daemon.h"
#include "libs/log4z.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <boost/lexical_cast.hpp> 

volatile bool g_quit = false;
volatile unsigned int g_ticks = 0;

void signal_handler(int sig)
{
	switch(sig)
	{
		case SIGTERM:
		case SIGINT:
		{
			g_quit = true;
			break;
		}
		case SIGALRM:
		{
			g_ticks ++;
			break;
		}
	}
}

/////////////////////////////////////////////////
int Application::main(int argc, char **argv)
{
	welcome();
	parse_args(argc, argv);
	init();

	write_pid();
	run();
	remove_pidfile();
	
	m_ptr_conf.reset();

	return 0;
}

void Application::usage(int argc, char **argv)
{
	printf("Usage:\n");
	printf("    %s [-d] /path/to/config.ini [-s start|stop|restart]\n", argv[0]);
	printf("Options:\n");
	printf("    -d    run as daemon\n");
	printf("    -s    option to start|stop|restart the server\n");
	printf("    -h    show this message\n");
}

void Application::parse_args(int argc, char **argv)
{
	for(int i=1; i<argc; i++)
	{
		std::string arg = argv[i];
		if(arg == "-d")
		{
			m_app_args.is_daemon = true;
		}
		else if(arg == "-v")
		{
			exit(0);
		}
		else if(arg == "-h")
		{
			usage(argc, argv);
			exit(0);
		}
		else if(arg == "-s")
		{
			if(argc > i + 1)
			{
				i ++;
				m_app_args.start_opt = argv[i];
			}
			else
			{
				usage(argc, argv);
				exit(1);
			}
			
			if(m_app_args.start_opt != "start" && m_app_args.start_opt != "stop" && m_app_args.start_opt != "restart")
			{
				usage(argc, argv);
				fprintf(stderr, "Error: bad argument: '%s'\n", m_app_args.start_opt.c_str());
				exit(1);
			}
		}else{
			m_app_args.conf_file = argv[i];
		}
	}

	if(m_app_args.conf_file.empty())
	{
		usage(argc, argv);
		exit(1);
	}
}

void Application::init()
{
	if(!is_file(m_app_args.conf_file.c_str()))
	{
		fprintf(stderr, "'%s' is not a file or not exists!\n", m_app_args.conf_file.c_str());
		exit(1);
	}

	m_ptr_conf.reset(new IniConfig(m_app_args.conf_file));
	if(!m_ptr_conf.get())
	{
		fprintf(stderr, "error loading conf file: '%s'\n", m_app_args.conf_file.c_str());
		exit(1);
	}

#if 0
	{
		std::string conf_dir = real_dirname(m_app_args.conf_file.c_str());
		if(chdir(conf_dir.c_str()) == -1)
		{
			fprintf(stderr, "error chdir: %s\n", conf_dir.c_str());
			exit(1);
		}
	}
#endif

	m_app_args.pidfile = m_ptr_conf->ReadProfileString("Application","pidfile","");

	if(m_app_args.start_opt == "stop")
	{
		kill_process();
		exit(0);
	}

	if(m_app_args.start_opt == "restart")
	{
		if(file_exists(m_app_args.pidfile))
		{
			kill_process();
		}
	}
	
	check_pidfile();

	std::string logconf = m_ptr_conf->ReadProfileString("Application","logconf","");
	if(!is_file(logconf.c_str()))
	{
		fprintf(stderr, "'%s' is not a file or not exists!\n", logconf.c_str());
		exit(1);
	}

	// WARN!!!
	// deamonize() MUST be called before any thread is created!
	if(m_app_args.is_daemon)
	{
		daemonize();
	}

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGALRM, signal_handler);

	zsummer::log4z::ILog4zManager::getRef().config(logconf.c_str());
	zsummer::log4z::ILog4zManager::getRef().start();
}

int Application::read_pid()
{
	if(m_app_args.pidfile.empty())
	{
		return -1;
	}

	std::string s;
	file_get_contents(m_app_args.pidfile, &s);
	if(s.empty())
	{
		return -1;
	}

	return boost::lexical_cast<int>(s);
}

void Application::write_pid()
{
	if(!m_app_args.is_daemon)
	{
		return;
	}

	if(m_app_args.pidfile.empty())
	{
		return;
	}

	int pid = (int)getpid();
	std::string s = boost::lexical_cast<std::string>(pid);
	int ret = file_put_contents(m_app_args.pidfile, s);
	if(ret == -1)
	{
		fprintf(stderr,"Failed to write pidfile '%s'(%s)", m_app_args.pidfile.c_str(), strerror(errno));
		exit(1);
	}
}

void Application::check_pidfile(){
	if(!m_app_args.is_daemon)
	{
		return;
	}

	if(m_app_args.pidfile.size())
	{
		if(access(m_app_args.pidfile.c_str(), F_OK) == 0)
		{
			fprintf(stderr, "Fatal error!\nPidfile %s already exists!\n"
				"Kill the running process before you run this command,\n"
				"or use '-s restart' option to restart the server.\n",
				m_app_args.pidfile.c_str());
			exit(1);
		}
	}
}

void Application::remove_pidfile()
{
	if(!m_app_args.is_daemon)
	{
		return;
	}

	if(m_app_args.pidfile.size())
	{
		remove(m_app_args.pidfile.c_str());
	}
}

void Application::kill_process()
{
	int pid = read_pid();
	if(pid == -1)
	{
		fprintf(stderr, "could not read pidfile: %s(%s)\n", m_app_args.pidfile.c_str(), strerror(errno));
		exit(1);
	}

	if(kill(pid, 0) == -1 && errno == ESRCH)
	{
		fprintf(stderr, "process: %d not running\n", pid);
		remove_pidfile();
		return;
	}
	
	int ret = kill(pid, SIGTERM);
	if(ret == -1)
	{
		fprintf(stderr, "could not kill process: %d(%s)\n", pid, strerror(errno));
		exit(1);
	}
	
	while(file_exists(m_app_args.pidfile))
	{
		usleep(100 * 1000);
	}
}

