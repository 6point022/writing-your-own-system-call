#include<stdio.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
#include<errno.h>
#include<unistd.h>

int main(int argc, char **argv) {
	int pid = atoi(argv[1]);
	long result = syscall(355, pid, argv[2]);
	if(result == 0)
		printf("System call sys_hello returned %ld\n", result);
	
	else {
		printf("Error :(\n");
		perror("Error ");
		printf("Error Number: %d\n", errno);
		
	}

	return 0;
}
