#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("alevasse");
MODULE_DESCRIPTION("Keyboard detection!");

static int	__init keyboard_detected(void)
{
	printk(KERN_INFO "Keyboard plugged in!\n");
	return 0;
}

static void      __exit clean(void)
{
        printk(KERN_INFO "Cleaning up module.\n");
}

module_init(keyboard_detected);
module_exit(clean);
