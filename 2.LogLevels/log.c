#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk

#define PROCFS_NAME "Log tester" 
#define SIZE 1024

static int __init helloInit(void)
{
    printk(KERN_INFO PROCFS_NAME ": Successfully loaded module %s\n", PROCFS_NAME);

    pr_emerg(PROCFS_NAME ": This is an emergency\n");
    pr_alert(PROCFS_NAME ": This is an alert\n");
    pr_crit(PROCFS_NAME ": This is a critical\n");
    for(int i=0;i<3;++i){
        pr_err_once(PROCFS_NAME ": Entering an error loop, this will be printed once\n");
        pr_err(PROCFS_NAME ": Error from error loop\n");
    }
    
    pr_warn(PROCFS_NAME ": This is a warning\n");
    pr_notice(PROCFS_NAME ": This is a notice\n");
    pr_info(PROCFS_NAME ": This is an info\n");
    pr_debug(PROCFS_NAME ": This is a debug message\n");
    return 0;
}

static void __exit helloExit(void)
{
    printk(KERN_INFO PROCFS_NAME ": Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(helloInit);
module_exit(helloExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("Module for testing logging types");
