#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include "sync.h"
#include "timer.h"

static void web_daemon()
{
//create child and wait for parent to terminate
	pid_t pid;
	pid = fork();

	if (pid < 0)
		exit(EXIT_FAILURE);
	
	if (pid > 0)
	{
		exit(EXIT_SUCCESS);
	}
	else if (pid == 0)
	{
	
		begin_log();
		
		umask(0);
		
		//close and dirs
		if (chdir("/") < 0 ) { exit(EXIT_FAILURE); }
		
		int x;//close file descriptors
		for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
		{
			close(x);
		}
		
		while (1)
		{
			if(signal(SIGINT, sig_handler) == SID_ERR)
			{
				//some error bois
			}
			if(timer() == 1)
			{
			//where the party is at
			
				char* args[] = {"chmod", "444",  "/var/www/Intranet/", NULL};
				execvp(args[0], args);
				syslog(LOG_ERR, "removing editing priveledges");
				audit_log();
				backup();
				site_update();
				char* args[] = {"chmod", "777",  "/var/www/Intranet/", NULL};
				execvp(args[0], args);
				syslog(LOG_ERR, "returning editing priveledges");
				
			}
		}
	}
		
}

void begin_log()
{
	char* args[] = {"auditctl", "-w",  "/var/www/Intranet/", "-p" "rwxa", NULL};
	execvp(args[0], args);
	syslog(LOG_ERR, "Audit failed or already running");
}

void audit_log()
{
	char* args[] = {"ausearch", "-f", "/var/www/html/Intranet/", "|", "aureport", "-f", "-l", ">", "/home/arek/Documents/site_logs.txt", NULL};
	execvp(args[0], args);
	syslog(LOG_ERR, "Log Audit failed");
}

void site_update()
{
	sync("/var/www/html/Intranet/", "/var/www/html/live/");
}

void backup()
{
	sync("/var/www/html/Intranet/","/var/www/html/backup/");
}

void sig_handler(int sigNum)
{
	if(sigNum == SIGINT || sigNum == SIGTERM)
	{
		syslof(LOG_ERR,"TErminating Daemon");
		exit(EXIT_SUCCESS);
	}
}

int main()
{
	web_daemon();
}