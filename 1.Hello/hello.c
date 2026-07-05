#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk

#define PROCFS_NAME "Hello sayer" 
#define SIZE 1024

static int __init helloInit(void)
{
    printk(KERN_INFO "Successfully loaded module %s\n", PROCFS_NAME);
    return 0;
}

static void __exit helloExit(void)
{
    printk(KERN_INFO "Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(helloInit);
module_exit(helloExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("First module on orange pi");
