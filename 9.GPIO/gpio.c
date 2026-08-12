#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk
#include <linux/gpio.h>

#define PROCFS_NAME "GPIO Bulb&Button" 
#define SIZE 1024

#define GPIO_OFFSET 0
#define ALLWINNER_PC_OFFSET 64
#define LED_GPIO 9
#define BUTTON_GPIO 11

static int led_gpio = (GPIO_OFFSET + ALLWINNER_PC_OFFSET + LED_GPIO);
static int button_gpio = (GPIO_OFFSET + ALLWINNER_PC_OFFSET + BUTTON_GPIO);

static int __init helloInit(void)
{
 
    int status = gpio_request(led_gpio, "led_gpio");
    if(status)
    {
        pr_err("Failed to request led gpio");
        return status;
    }

    gpio_direction_output(led_gpio, 1);

    status = gpio_request(button_gpio, "button_gpio");
    if(status)
    {
        pr_err("Failed to request button gpio");
        return status;
    }

    gpio_direction_input(button_gpio);

    pr_alert("Button state is: %d\n", gpio_get_value(button_gpio));

    printk(KERN_INFO "Successfully loaded module %s\n", PROCFS_NAME);
    return 0; 
}

static void __exit helloExit(void)
{
     gpio_set_value(led_gpio, 0);
    gpio_free(led_gpio);
    gpio_free(button_gpio);
    printk(KERN_INFO "Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(helloInit);
module_exit(helloExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("First module on orange pi");
