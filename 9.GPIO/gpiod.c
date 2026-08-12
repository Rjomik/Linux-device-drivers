#include <linux/module.h> 
#include <linux/kern_levels.h> //KERN_ERR, KERN_INFO 
#include <linux/kernel.h> //printk
#include <linux/gpio/consumer.h>

static struct gpio_desc *led, *button;

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
    led = gpio_to_desc(led_gpio);
    if(!led)
    {
        pr_err("Failed to request led gpio");
        return -1;
    }
    gpiod_direction_output(led, 1);

    button = gpio_to_desc(button_gpio);
    if(!button)
    {
        pr_err("Failed to request button gpio");
        return -2;
    }
    gpiod_direction_input(button);
    pr_alert("Button state is: %d\n", gpiod_get_value(button));

    printk(KERN_INFO "Successfully loaded module %s\n", PROCFS_NAME);

    return 0;
}

static void __exit helloExit(void)
{
    gpiod_set_value(led, 0);
    printk(KERN_INFO "Successfully unloaded module %s\n", PROCFS_NAME);
}
module_init(helloInit);
module_exit(helloExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rjomik");
MODULE_DESCRIPTION("First module on orange pi");
