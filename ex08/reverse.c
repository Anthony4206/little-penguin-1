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

static ssize_t myfd_read(struct file *fp, char __user *user,
			 size_t size, loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user,
			  size_t size, loff_t *offs);

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

static char	str[PAGE_SIZE] = {0};

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

ssize_t myfd_read(struct file *fp, char __user *user,
		  size_t size, loff_t *offs)
{
	size_t	len;
	size_t	i;
	size_t	ret;
	char	*tmp;

	if (*offs != 0)
		return 0;

	len = strlen(str);
	tmp = kmalloc(len + 1, GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;

	for (i = 0; i < len; i++)
		tmp[i] = str[len - 1 - i];
	tmp[i] = '\0';
	ret = simple_read_from_buffer(user, size, offs, tmp, i);
	*offs = len;
	kfree(tmp);
	return ret;
}

ssize_t myfd_write(struct file *fp, const char __user *user,
		   size_t size, loff_t *offs)
{
	ssize_t res;

	if (size >= PAGE_SIZE)
		return -EINVAL;

	res = simple_write_to_buffer(str, size, offs, user, size);
	if (res >= 0)
		str[size + 1] = '\0';
	return res;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
