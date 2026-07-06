#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk

#define PROCFS_NAME "CharDevice" 

const struct file_operations fop = {

};
static int major;
static int __init helloInit(void)
{
    major = register_chrdev(0, PROCFS_NAME, &fop);
    if(major < 0){
        pr_err("Failed to register char device\n");
        return major;
    }
    pr_info("Given major number:%d\n", major);
    printk(KERN_INFO PROCFS_NAME ": Successfully loaded module %s\n", PROCFS_NAME);
    return 0;
}

static void __exit helloExit(void)
{
    unregister_chrdev(major, PROCFS_NAME);
    printk(KERN_INFO PROCFS_NAME ": Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(helloInit);
module_exit(helloExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("Module creating simple character device");
