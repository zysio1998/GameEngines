#include <errno.h>
#include <mqueue.h>
#include <stdbool.h>

bool sync(char source[], char destination[])
{
	if(strlen(src) == 0 || strlen(dest) == 0)
	{
		exit(EXIT_FAILURE);
	};
		
	char* args[] =  {"/usr/bin/rsync", "-a", src, dest, NULL};
	syslog(LOG_INFO, "Executing Update on %s %s", src, dest);
	//execvp(args[0], args);
	//syslog(LOG_ERR, "Failed to execute %s %s", src, dest);
}