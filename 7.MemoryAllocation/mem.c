#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk
#include <linux/cdev.h>
#include <linux/slab.h> //k memory
#include <linux/vmalloc.h>

#define PROCFS_NAME "MemoryAlloc" 



#define BUFFER_SIZE 64

struct myStruct{
    int someInt;
    char someString[50];
};

static char *buffer1, *buffer2, *buffer3;
static struct kmem_cache* myCache;
static struct myStruct** item;
static void dummyConstruct(void* param){
    
}
static int __init myInit(void)
{
    int successfullyAllocated = 0;
    buffer1 = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if(!buffer1){
        pr_err("Failed to allocate with kmalloc\n");
        return -1;
    }
    pr_info("Allocated region using kmalloc. \n\tVirtual address: %px\n\tPhysical address:%llx\n", buffer1, virt_to_phys(buffer1));
    buffer2 = kzalloc(BUFFER_SIZE, GFP_KERNEL);
    if(!buffer2){
        pr_err("Failed to allocate with kzalloc\n");
        goto freeBuffer1;
    }
    pr_info("Allocated region using kzalloc. \n\tVirtual address: %px\n\tPhysical address:%llx\n", buffer2, virt_to_phys(buffer2));
    buffer3 = vmalloc(BUFFER_SIZE);
    if(!buffer3){
        pr_err("Failed to allocate with vmalloc\n");
        goto freeBuffer2;
    }
    pr_info("Allocated region using vmalloc. \n\tVirtual address: %px\n\tPhysical address:%llx\n", buffer3, virt_to_phys(buffer3));
    myCache = kmem_cache_create("myCustomCache", sizeof(struct myStruct), 0, SLAB_HWCACHE_ALIGN, dummyConstruct);
    if(!myCache){
        pr_err("Failed to create cache\n");
        goto freeBuffer3;
    }
    pr_info("Created cache\n");
    item = vmalloc(sizeof(struct myStruct*) * BUFFER_SIZE);
    
    for(int i=0;i<BUFFER_SIZE;i++){
        item[i] = kmem_cache_alloc(myCache, GFP_KERNEL);
        if(item[i]){
            item[i]->someInt = 4558+i;
            snprintf(item[i]->someString, 50, "My string for the item %d", i);
            ++successfullyAllocated;
        }
        
    }
    pr_info("Allocated slab using cache. N of items:%d\n", successfullyAllocated);
    
    
    
    return 0;

    freeBuffer3:
    vfree(buffer3);

    freeBuffer2:
    kfree(buffer2);

    freeBuffer1:
    kfree(buffer1);

    return -1;


    printk(KERN_INFO PROCFS_NAME ": Successfully loaded module %s\n", PROCFS_NAME);
    return 0;
}

static void __exit myExit(void)
{
    for(int i=0;i<BUFFER_SIZE;i++){
        if(item[i])
            kmem_cache_free(myCache, item[i]);
    }
    vfree(item);
    kmem_cache_destroy(myCache);
    vfree(buffer3);
    kfree(buffer2);
    kfree(buffer1);
    
    
    
    
    printk(KERN_INFO PROCFS_NAME ": Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(myInit);
module_exit(myExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("Module for testing kernel memory allocation");
