#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("alevasse");
MODULE_DESCRIPTION("Misc Driver!");

static char *login = "alevasse";
#define LOGIN_LEN 8

static ssize_t
ft_write(struct file *f, char __user *buf, size_t c, loff_t *o)
{
	char kbuf[LOGIN_LEN + 1] = {0};
	
	if (c != LOGIN_LEN) {
		printk(KERN_ERR "Error: Bad length!\n");
		return -EINVAL;
	}
	if (copy_from_user(kbuf, buf, LOGIN_LEN)) {
		printk(KERN_ERR "Error: Copy from user failed!\n");
		return -EFAULT;
	}
	if (strncmp(&buf, login, LOGIN_LEN)) {
		printk(KERN_INFO "Success: Right data received from user!\n");
		return LOGIN_LEN;	
	}
	printk(KERN_ERR "Error: Bad login name!\n");
	return -EINVAL;
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
