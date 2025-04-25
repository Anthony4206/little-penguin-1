#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("alevasse");
MODULE_DESCRIPTION("Misc Driver!");

static ssize_t
ft_write(struct file *f, char __user *b, size_t c, loff_t *o)
{
	char	*login = "alevasse";
	int	len = 8;

	if (*buf == "alevasse") {
	
	}
}

static ssize_t
ft_read(struct file *f, const char __user *b, size_t c, loff_t *o)
{
	char	*login = "alevasse";
	int	len = 
}

static struct file_operations	s_fops = {
	.owner	= THIS_MODULE,
	.write	= ft_write,
	.read	= ft_read
}
static struct miscdevice	s_misc_device = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "fortytwo",
	.fops	= &fops
};

static int	__init misc_init(void)
{
	int error;

	error = misc_register(&s_misc_device);
	if (error) {
		printk(KERN_ERR "Misc device register failed!\n");
		return error;
	}
	printk(KERN_INFO "Success: Misc device has been registered!\n");
	return 0;
}

static void      __exit misc_exit(void)
{
	misc_deregister(&s_misc_device);
        printk(KERN_INFO "Misc device has been deregistered!\n");
}

module_init(misc_init);
module_exit(misc_exit);
