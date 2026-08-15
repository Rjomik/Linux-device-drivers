#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/timer.h>

#define PROCFS_NAME "GPIO_D Button interrupt handling" 
#define SIZE 1024

#define GPIO_OFFSET 0
#define ALLWINNER_PC_OFFSET 64
#define LED_GPIO 9
#define BUTTON_GPIO 11

static struct gpio_desc *led, *button;

static int led_gpio = (GPIO_OFFSET + ALLWINNER_PC_OFFSET + LED_GPIO);
static int button_gpio = (GPIO_OFFSET + ALLWINNER_PC_OFFSET + BUTTON_GPIO);

static int irqNum = 0;
struct timer_list debounceTimer;
#define DEBOUNCE_DELAY_MS 20
static irqreturn_t buttonHandler(int irq, void *dev)
{
    mod_timer(&debounceTimer, jiffies+msecs_to_jiffies(DEBOUNCE_DELAY_MS));
    return IRQ_HANDLED;
}

static void DebounceCallback(struct timer_list *tm){
    pr_info("Setting button value\n");
    gpiod_set_value(led, !gpiod_get_value(led));
}

static int __init helloInit(void)
{
    int status;
    led = gpio_to_desc(led_gpio);
    if(!led)
    {
        pr_err("Failed to request led gpio");
        return -1;
    }

    gpiod_direction_output(led, 0);

    button = gpio_to_desc(button_gpio);
    if(!button)
    {
        pr_err("Failed to request button gpio");
        return -2;
    }

    gpiod_direction_input(button);

    irqNum = gpiod_to_irq(button);
    if(irqNum < 0)
    {
        pr_err("Failed to get the IRQ num from GPIO num");
        return -3;
    }

    status = request_irq(irqNum, buttonHandler, IRQF_TRIGGER_FALLING, "My little button", NULL);
    if(status)
    {
        pr_err("Failed to request an interrupt");
        return -status;
    }


    pr_alert("The interrupt number is: %d\n", irqNum);

    timer_setup(&debounceTimer, DebounceCallback, 0);

    printk(KERN_INFO "Successfully loaded module %s\n", PROCFS_NAME);
    return 0; 
}

static void __exit helloExit(void)
{
    gpiod_set_value(led, 0);
    free_irq(irqNum, NULL);
    del_timer_sync(&debounceTimer);
    printk(KERN_INFO "Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(helloInit);
module_exit(helloExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("First module on orange pi");
