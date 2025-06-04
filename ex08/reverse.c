// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anthony Levasseur <anthony@ne02ptzero.me>");
MODULE_DESCRIPTION("Reverse string module");

static char str[PAGE_SIZE];

ssize_t myfd_read(struct file *fp, char __user *user,
		  size_t size, loff_t *offs)
{
	size_t t;
	size_t i;
	char *tmp2;

	// Malloc like a boss
	tmp2 = kmalloc(sizeof(char), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;

	for (i = 0; i < len; i++)
		tmp[i] = str[len - i - 1];
	tmp[i] = '\0';

	if (copy_to_user(user_buf, tmp, len)) {
		kfree(tmp);
		return -EFAULT;
	}

	*ppos = len;
	kfree(tmp);
	return len;
}

ssize_t myfd_write(struct file *fp, const char __user *user,
		   size_t size, loff_t *ppos)
{
	ssize_t res;

	res = 0;
	res = simple_write_to_buffer(str, size, offs, user, size) + 1;
	// 0x0 = '\0'
	str[size + 1] = 0x0;
	return res;
}

static const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = myfd_read,
	.write = myfd_write
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};

static int __init myfd_init(void)
{
	int retval;

	retval = misc_register(&myfd_device);
	return retval;
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
}

module_init(myfd_init);
module_exit(myfd_cleanup);
