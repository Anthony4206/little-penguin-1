// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");

#define DEVICE_NAME "reverse"

static char str[PAGE_SIZE];

static ssize_t myfd_read(struct file *file, char __user *user_buf,
                         size_t count, loff_t *ppos)
{
	size_t len = strlen(str);
	size_t i;
	char *tmp;

	if (*ppos != 0)
		return 0;

	tmp = kmalloc(len + 1, GFP_KERNEL);
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

static ssize_t myfd_write(struct file *file, const char __user *user_buf,
                          size_t count, loff_t *ppos)
{
	ssize_t ret;

	if (count >= PAGE_SIZE)
		return -EINVAL;

	memset(str, 0, sizeof(str));

	ret = simple_write_to_buffer(str, PAGE_SIZE - 1, ppos, user_buf, count);
	if (ret < 0)
		return ret;

	str[ret] = '\0';
	return ret;
}

static const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = myfd_read,
	.write = myfd_write,
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &myfd_fops,
};

static int __init myfd_init(void)
{
	int ret;

	ret = misc_register(&myfd_device);
	if (ret) {
		pr_err("Failed to register misc device\n");
		return ret;
	}

	pr_info("reverse module loaded\n");
	return 0;
}

static void __exit myfd_exit(void)
{
	misc_deregister(&myfd_device);
	pr_info("reverse module unloaded\n");
}

module_init(myfd_init);
module_exit(myfd_exit);
