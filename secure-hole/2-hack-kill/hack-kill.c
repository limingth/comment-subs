
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>

#include <linux/sched.h>

//#define SYSCALL_NUM	__NR_getdents64
#define SYSCALL_NUM	__NR_kill

MODULE_AUTHOR("AKAEDU");
MODULE_DESCRIPTION("module example ");
MODULE_LICENSE("GPL");

// see all syscall number in 
// /usr/src/linux-headers-3.2.0-29-generic-pae/arch/x86/include/asm/unistd_32.h 

//extern void* sys_call_table[]; /*sys_call_table 被引出，所以我们可访问它*/
void ** sys_call_table = (int *)0xc15b0000;
int (*orig_syscall)(int pid, int sig, ...); /*未改前的系统调用*/

long hacked_cmd(int pid, int sig_no, ...)
{
	struct task_struct *ptr = current;
	int ret;

	printk("kill para pid = %d! sig = %d\n", pid, sig_no);

	if ((pid == ptr->pid) && (sig_no == SIGTERM))
	{
		printk("kill is hacked!\n");
		printk("Use kill -9 to kill your bash!\n");
		return 0;
	}

	ret = (*orig_syscall)(pid, sig_no);
	printk("ret = %d\n", ret);
	return	ret;
}

int orig_cr0;

unsigned int clear_and_return_cr0(void)
{
	unsigned int cr0 = 0;
	unsigned int ret;

	asm volatile("movl %%cr0,%%eax"
			:"=a"(cr0)
		    );                                    //存储cr0的值
	ret = cr0;

	/*将cr0的第16位清零，第16为0表示允许超级权限*/
	cr0 &= 0xfffeffff;
	asm volatile("movl %%eax,%%cr0"
			:
			:"a"(cr0)
		    );
	return ret;
}

//恢复cr0寄存器的第16位
void setback_cr0(unsigned int val)
{
	asm volatile("movl %%eax,%%cr0"
			:
			:"a"(val)
		    );    
}

int my_init_module(void) /*模块初始化*/
{
	printk("init ok, SYSCALL NUM = %d\n", SYSCALL_NUM);
	printk("table at %p\n", sys_call_table);

	orig_cr0 = clear_and_return_cr0(); //cr0寄存器的第16位清0

	orig_syscall = sys_call_table[SYSCALL_NUM];

	sys_call_table[SYSCALL_NUM] = hacked_cmd;

	//恢复cr0寄存器的第16位    
	setback_cr0(orig_cr0);

	return 0;
}

void my_cleanup_module(void) /*模块卸载*/
{
	printk("exit ok\n");

	orig_cr0 = clear_and_return_cr0(); //cr0寄存器的第16位清0

	sys_call_table[SYSCALL_NUM] = orig_syscall; /*把系统调用恢复*/

	//恢复cr0寄存器的第16位    
	setback_cr0(orig_cr0);
}


module_init(my_init_module);
module_exit(my_cleanup_module);
