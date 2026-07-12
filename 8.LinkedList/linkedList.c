#include <linux/module.h> 
#include <linux/init.h> //module_init / exit
#include <linux/list.h>


#define PROCFS_NAME "LinkedList" 
#define BUFFER_SIZE 64

struct myStruct{
    int someInt;
    char someString[50];
    struct list_head node;
};

static LIST_HEAD(myList);

static int __init myInit(void)
{
    struct myStruct *tmp, *next;
    struct list_head *ptr;

    pr_info("Adding 3 items to the list\n");
    tmp = kmalloc(sizeof(struct myStruct), GFP_KERNEL);
    if(!tmp){
        pr_err("Failed\n");
        goto memFree;
    }
    list_add_tail(&tmp->node,&myList);
    tmp->someInt = 1;
    strcpy(tmp->someString, "This one added after list_add_tail");
    //
    tmp = kmalloc(sizeof(struct myStruct), GFP_KERNEL);
    if(!tmp){
        pr_err("Failed\n");
        goto memFree;
    }
    tmp->someInt = 2;
    strcpy(tmp->someString, "This one added before list_add_tail");
    list_add_tail(&tmp->node,&myList);
    
    //
    tmp = kmalloc(sizeof(struct myStruct), GFP_KERNEL);
    if(!tmp){
        pr_err("Failed\n");
        goto memFree;
    }
    list_add_tail(&tmp->node,&myList);
    tmp->someInt = 3;
    strcpy(tmp->someString, "Third item");

    list_for_each(ptr, &myList){
        tmp = list_entry(ptr, struct myStruct, node);
        pr_alert("Node. int=%d; string=%s\n", tmp->someInt, tmp->someString);
    }

    printk(KERN_INFO PROCFS_NAME ": Successfully loaded module %s\n", PROCFS_NAME);
    return 0;

    memFree:
    list_for_each_entry_safe(tmp, next, &myList, node){
        pr_crit("Deleting Node. int=%d\n", tmp->someInt);
        list_del(&tmp->node);
        kfree(tmp);
    }
    return -1;
}

static void __exit myExit(void)
{
    struct myStruct *tmp, *next;
    list_for_each_entry_safe(tmp, next, &myList, node){
        pr_crit("Deleting Node. int=%d\n", tmp->someInt);
        list_del(&tmp->node);
        kfree(tmp);
    }
    printk(KERN_INFO PROCFS_NAME ": Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(myInit);
module_exit(myExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("Module for testing kernel level linked list");
