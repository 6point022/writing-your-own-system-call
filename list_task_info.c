#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/syscalls.h>
#include<linux/file.h>
#include<asm/uaccess.h>
#include<uapi/asm-generic/errno-base.h>
#include<linux/fs.h>
#include<linux/fcntl.h>

#include "taskinfo.h"

int write_to_file(struct file *file, char *data) {
	file->f_op->write(file, data, strlen(data), &file->f_pos);
	return 0;
}

asmlinkage long sys_sh_task_info(long pid, char *filename) {
	
	if(pid <= 0 || pid > 32768)
		return -EINVAL; // invalid argument	
	
	struct task_struct *task;
	struct file *file;
	
	char data[400], temp[400];
	
	loff_t pos = 0;
	int fd;

	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);

	fd = sys_open(filename, O_WRONLY|O_CREAT, 0644);
	

	for_each_process(task) {
		
		if(pid == (long) task->pid) {
			printk("Process: %s\nPID_Number: %ld\nProcess State: %ld\nPriority: %ld\n", task->comm, (long) task->pid, (long) task->state, (long) task->prio);


			sprintf(temp, "Process %s\nPID_Number: %ld\nProcess State: %ld\nPriority: %ld\n", task->comm, (long) task->pid, (long) task->state, (long) task->prio);						
		
			strcat(data, temp);

			
			if(fd < 0)
				return -EISDIR; // you cannot write to a directory

	
			file = fget(fd);
			write_to_file(file, data);
			
		}
	
	}

	
	set_fs(old_fs);
	return 0;
}
