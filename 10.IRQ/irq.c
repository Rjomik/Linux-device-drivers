#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define PROCFS_NAME "GPIO Button interrupt handling" 
#define SIZE 1024

#define GPIO_OFFSET 0
#define ALLWINNER_PC_OFFSET 64
#define LED_GPIO 9
#define BUTTON_GPIO 11

static int led_gpio = (GPIO_OFFSET + ALLWINNER_PC_OFFSET + LED_GPIO);
static int button_gpio = (GPIO_OFFSET + ALLWINNER_PC_OFFSET + BUTTON_GPIO);

static int irqNum = 0;

static irqreturn_t buttonHandler(int irq, void *dev)
{
    pr_alert("Button was pressed\n");
    gpio_set_value(led_gpio, !gpio_get_value(led_gpio));
    return IRQ_HANDLED;
}

static int __init helloInit(void)
{
 
    int status = gpio_request(led_gpio, "led_gpio");
    if(status)
    {
        pr_err("Failed to request led gpio");
        return -status;
    }

    gpio_direction_output(led_gpio, 0);

    status = gpio_request(button_gpio, "button_gpio");
    if(status)
    {
        pr_err("Failed to request button gpio");
        return -status;
    }

    gpio_direction_input(button_gpio);

    irqNum = gpio_to_irq(button_gpio);
    if(irqNum < 0)
    {
        pr_err("Failed to get the IRQ num from GPIO num");
        return -status;
    }

    status = request_irq(irqNum, buttonHandler, IRQF_TRIGGER_FALLING, "My little button", NULL);
    if(status)
    {
        pr_err("Failed to request an interrupt");
        return -status;
    }


    pr_alert("The interrupt number is: %d\n", irqNum);

    printk(KERN_INFO "Successfully loaded module %s\n", PROCFS_NAME);
    return 0; 
}

static void __exit helloExit(void)
{
    gpio_set_value(led_gpio, 0);
    gpio_free(led_gpio);
    gpio_free(button_gpio);
    free_irq(irqNum, NULL);
    printk(KERN_INFO "Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(helloInit);
module_exit(helloExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("First module on orange pi");
