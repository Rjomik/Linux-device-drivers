#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk

#define PROCFS_NAME "Params" 
#define DEFAULT_PARAM_SIZE 32

static int intParam = 10;
static bool boolParam = false;
static char mutableStr[DEFAULT_PARAM_SIZE] = "You should have changed this";
static int integerArray[DEFAULT_PARAM_SIZE] = {1,2,3};
static int integerArraySize = 3;

module_param(intParam, int, 0444);
MODULE_PARM_DESC(intParam, "Just an integer");

module_param(boolParam, bool, 0444);
MODULE_PARM_DESC(boolParam, "Just a boolean");

module_param_string(Exposed_name, mutableStr, DEFAULT_PARAM_SIZE, 0444);
MODULE_PARM_DESC(Exposed_name, "Just a mutable string");

module_param_array(integerArray, int, &integerArraySize, 0444);
MODULE_PARM_DESC(integerArray, "Just an integer array");

static int __init helloInit(void)
{
    printk(KERN_INFO PROCFS_NAME ": Successfully loaded module %s\n", PROCFS_NAME);
    pr_info("%s: intParam: %d\n", PROCFS_NAME, intParam);
    pr_info("%s: boolParam: %d\n", PROCFS_NAME, boolParam);
    pr_info("%s: Mutable string: %s\n", PROCFS_NAME, mutableStr);

    pr_info("%s: integer array: ", PROCFS_NAME);
    for(int i=0;i<DEFAULT_PARAM_SIZE;i++){
        pr_cont("%d ", integerArray[i]);
    }
    pr_cont("\n");

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
