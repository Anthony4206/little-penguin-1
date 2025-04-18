#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("alevasse");
MODULE_DESCRIPTION("Hello World!");

static int	__init hello(void)
{
	printk(KERN_INFO "Hello world !\n");
	return 0;
}

static void      __exit clean(void)
{
        printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello);
module_exit(clean);
