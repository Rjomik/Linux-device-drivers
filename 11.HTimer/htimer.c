#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk
#include <linux/hrtimer.h>

#define PROCFS_NAME "Hight resolution timer tester" 
#define SIZE 1024

static struct hrtimer myTimer;
static ktime_t period;
static u64 timerStart;
static enum hrtimer_restart	timerFunction(struct hrtimer *tm)
{
    //hrtimer_forward_now(&myTimer, period); 
    //return HRTIMER_RESTART;
    pr_alert("Elapsed: %llu\n",  ktime_get_ns() - timerStart);
    return HRTIMER_NORESTART;
}


static int __init helloInit(void)
{
    hrtimer_init(&myTimer, CLOCK_MONOTONIC,  HRTIMER_MODE_REL);
    myTimer.function = timerFunction;
    timerStart = ktime_get_ns();
    period = ktime_set(1,0);
    hrtimer_start(&myTimer,period,HRTIMER_MODE_REL);
    printk(KERN_INFO "Successfully loaded module %s\n", PROCFS_NAME);
    return 0;
}

static void __exit helloExit(void)
{
    hrtimer_cancel(&myTimer);
    printk(KERN_INFO "Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(helloInit);
module_exit(helloExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("");
