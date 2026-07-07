#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h> //to/from user
#include <linux/slab.h> //k memory


#define PROCFS_NAME "Custom_FOP_Device" 

static struct mutex myMutex;

int myOpen(struct inode *physicalFile, struct file *fileDescriptor){
    pr_info("File has been opened\n");
    pr_info("\tMode:%#X\n", fileDescriptor->f_mode);
    pr_info("\tPosition:%lld\n", fileDescriptor->f_pos);
    pr_info("\tFlags:%#X\n", fileDescriptor->f_flags);
    return 0;
}

int myRelease(struct inode *physicalFile, struct file *fileDescriptor){
    pr_info("File has been released\n");
    return 0;
}

#define BUFFER_SIZE 64
static char *buffer;

ssize_t myRead(struct file *fileDescriptor, char __user *userBuffer, size_t count, loff_t *offset)
{
    size_t toCopy, copied, notCopied;
    pr_info("Got a read request. Count=%zu Offset=%lld\n", count, *offset);
    if(*offset >= BUFFER_SIZE)
    {
        pr_warn("Somehow got an out of bound offset\n");
        return 0;
    }

    if(mutex_lock_interruptible(&myMutex)){
        return -ERESTARTSYS;
    }
    
    toCopy = *offset + count >= BUFFER_SIZE? BUFFER_SIZE - *offset : count;
    notCopied = copy_to_user(userBuffer, buffer + *offset, toCopy);
    copied = toCopy - notCopied;
    if(notCopied>0)
    {
        pr_warn("Managed to copy only:%zu bytes\n", copied);
    }
    *offset += copied;

    mutex_unlock(&myMutex);

    return copied;
}

ssize_t myWrite(struct file *fileDescriptor, const char __user *userBuffer, size_t count, loff_t *offset)
{
    size_t written, notWritten;

    pr_info("Got a write request. Count=%zu Offset=%lld\n", count, *offset);
    if(*offset + count >= BUFFER_SIZE)
    {
        pr_err("Not enough space\n");
        return ENOMEM;
    }

    if(mutex_lock_interruptible(&myMutex)){
        return -ERESTARTSYS;
    }
    
    notWritten = copy_from_user(buffer + *offset, userBuffer, count);
    written = count - notWritten;
    if(notWritten>0)
    {
        pr_warn("Managed to write only:%zu bytes\n", written);
    }
    *offset += written;

    mutex_unlock(&myMutex);

    return written;
}

const struct file_operations fop = {
    .open = &myOpen,
    .release = &myRelease,
    .read = &myRead,
    .write = &myWrite
};

struct class *deviceClass;
static dev_t deviceNumber;
static struct cdev myCdev;
static int __init myInit(void)
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

    buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if(!buffer){
        status = -3;
        pr_err("Failed to allocate buffer\n");
        goto deleteClass;
    }

    memset(buffer, 0, BUFFER_SIZE);

    mutex_init(&myMutex);

    printk(KERN_INFO PROCFS_NAME ": Successfully loaded module %s\n", PROCFS_NAME);
    return 0;
    
    //cleanup section
    deleteClass:
    class_destroy(deviceClass);

    deleteCdev:
    cdev_del(&myCdev);

    freeRegion:
    unregister_chrdev_region(deviceNumber, 1);

    return status;
}

static void __exit myExit(void)
{
    mutex_destroy(&myMutex);
    kfree(buffer);
    device_destroy(deviceClass, deviceNumber);
    class_destroy(deviceClass);
    cdev_del(&myCdev);
    unregister_chrdev_region(deviceNumber, 1);
    printk(KERN_INFO PROCFS_NAME ": Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(myInit);
module_exit(myExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("Module creating simple character device with custom file operation");
