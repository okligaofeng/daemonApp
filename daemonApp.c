#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>

pid_t getPidByName(char *name)
{
	DIR	*dirHandle;
	pid_t  pid = -1;
	FILE  *fp;	
	char  StrPathName[100];	
	char  buff[100];
	struct dirent *ptr;
	
	memset(StrPathName, '0', 100);

	if ( (dirHandle = opendir("/proc")) ==NULL ) {
		perror("opendir /proc");
		return -1;
	}
	chdir("/proc");
	while( (ptr = readdir(dirHandle)) != NULL) {
  	        if( (strcmp(ptr->d_name, ".") == 0)  ||  (strcmp(ptr->d_name, "..") == 0) ) 
			continue;
		if( DT_DIR != ptr->d_type)
			continue;
	
 	  	sprintf(StrPathName, "/proc/%s/cmdline", ptr->d_name);
		printf("%s, %d, %s\n", __FUNCTION__, __LINE__, StrPathName);;
	   	if( (fp = fopen(StrPathName, "r") ) == NULL) {
			    perror("fopen StrPathName");
			    continue;
		}
	   	memset(buff,'0',100);
	   	if( (int)fread(buff, 100, 1, fp) == -1) {
			perror("fread StrPathName");
			fclose(fp);
		        continue;
		}
		if( strcmp(buff, name) == 0) {
			pid = (pid_t)atoi(ptr->d_name);
		}
		fclose(fp);
	}
	 closedir(dirHandle);
	 return pid;	
}

void doSomething(pid_t pid, char *name)
{
	char * argv[] = {name, (char *)0 };
#if 0
	int ppid;
	if ( kill(pid, 0) == -1 ) {
		/* progress don't have */
		chdir("/root/liftdisplaypanel2");
		printf("App_Name = %s\n", name);		

		if ((ppid = fork()) < 0)
			perror("fork once");
		else if (pid != 0) {			/* once parent */
			exit(0);
		}		
		
		if ((ppid = vfork()) < 0)
				perror("fork twice");
		else if (ppid == 0)	 {			/* twice child */
			execv(name, argv);
			exit(0);
		}			
	}
#else 
	chdir("/root/liftdisplaypanel2");
	system(*argv);
#endif	
}
int main (int argc, char *argv[])
{
	pid_t	pid;
	char App[50];

	memset(App, '0', 50);
 	sprintf(App, "./%s", argv[1]);

	daemon(1, 0);
	while(1) {
		pid = getPidByName(App);	
		printf("APP pid = %d\n", pid);
		if ( pid == -1)
			doSomething(pid, App);
		sleep(10);
	}	
}
