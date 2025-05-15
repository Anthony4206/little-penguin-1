#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("alevasse");
MODULE_DESCRIPTION("Misc Driver!");

static char *login = "alevasse";
#define LOGIN_LEN 8

static ssize_t
ft_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char kbuf[LOGIN_LEN + 1] = {0};
	
	if (count != LOGIN_LEN) {
		printk(KERN_ERR "Error: Bad length!\n");
		return -EINVAL;
	}
	if (copy_from_user(kbuf, buf, LOGIN_LEN)) {
		printk(KERN_ERR "Error: Copy from user failed!\n");
		return -EFAULT;
	}
	if (strncmp(kbuf, login, LOGIN_LEN) == 0) {
		printk(KERN_INFO "Success: Right data received from user!\n");
		return LOGIN_LEN;	
	}
	printk(KERN_ERR "Error: Bad login name!\n");
	return -EINVAL;
}

static ssize_t
ft_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	if (*ppos > 0) {
		printk(KERN_ERR "Error: EOF reached!\n");
		return 0;
	}
	if (count < LONGIN_LEN) {
		printk(KERN_ERR "Error: Count to small!\n");
		return -EINVAL;
	}
	if (copy_to_user(buf, login, LOGIN_LEN)) {
		printk(KERN_ERR "Error: Copy to user failed!\n");
		return -EFAULT;
	}
	printk(KERN_INFO "Success: Login copied to user buffer!\n");
	return LOGIN_LEN;
}

static struct file_operations	s_fops = {
	.owner	= THIS_MODULE,
	.write	= ft_write,
	.read	= ft_read
};

static struct miscdevice	s_misc_device = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "fortytwo",
	.fops	= &s_fops
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
