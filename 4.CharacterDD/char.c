#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk
#include <linux/fs.h>
#include <linux/cdev.h>


#define PROCFS_NAME "CharDevice" 

const struct file_operations fop = {

};
struct class *deviceClass;
static dev_t deviceNumber;
static struct cdev myCdev;
static int __init helloInit(void)
{
    int status = alloc_chrdev_region(&deviceNumber, 0, 1, PROCFS_NAME);
    if(status!=0){
        pr_err("Failed to allocate Major and Minor numbers\n");
        return status;
    }

    pr_info("Given major:%d minor:%d\n", MAJOR(deviceNumber), MINOR(deviceNumber));

    cdev_init(&myCdev, &fop);
    status = cdev_add(&myCdev, deviceNumber, 1);
    if(status < 0){
        pr_err("Failed to create the cdev\n");
        goto freeRegion;
    }

    deviceClass = class_create(THIS_MODULE, PROCFS_NAME);
    if(!deviceClass){
        status = -1;
        pr_err("Failed to create the class\n");
        goto deleteCdev;
    }

    if(!device_create(deviceClass, NULL, deviceNumber, NULL, PROCFS_NAME)){
        status = -2;
        pr_err("Failed to create the device\n");
        goto deleteClass;
    }






    printk(KERN_INFO PROCFS_NAME ": Successfully loaded module %s\n", PROCFS_NAME);
    return 0;

    deleteClass:
    class_destroy(deviceClass);

    deleteCdev:
    cdev_del(&myCdev);

    freeRegion:
    unregister_chrdev_region(deviceNumber, 1);

    return status;
}

static void __exit helloExit(void)
{
    device_destroy(deviceClass, deviceNumber);
    class_destroy(deviceClass);
    cdev_del(&myCdev);
    unregister_chrdev_region(deviceNumber, 1);
    printk(KERN_INFO PROCFS_NAME ": Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(helloInit);
module_exit(helloExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("Module creating simple character device");
