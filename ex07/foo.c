#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/capability.h>

static char	foo_data[PAGE_SIZE];
static size_t	foo_len;
static		DEFINE_MUTEX(foo_lock);

static ssize_t
foo_read(struct file *file, char __user *u_buf, size_t count, loff_t *ppos)
{
	ssize_t	ret;

	mutex_lock(&foo_lock);
	ret = simple_read_from_buffer(u_buf, count, ppos, foo_data, foo_len);
	printk(KERN_INFO "Success: foo_data copied to user buffer!\n");
	mutex_unlock(&foo_lock);
	return ret;
}

static ssize_t
foo_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;
	if (count > PAGE_SIZE)
		return -EINVAL;

	mutex_lock(&foo_lock);
	memset(foo_data, 0, PAGE_SIZE);

	if (copy_from_user(foo_data, buf, count)) {
		 mutex_unlock(&foo_lock);
		 printk(KERN_ERR "Error: Copy from user failed!\n");
		 return -EFAULT;
	}
	foo_len = count;
	printk(KERN_INFO "Success: foo_data received from root!\n");
	mutex_unlock(&foo_lock);
	return count;
}

const struct file_operations	s_fops_foo = {
	.owner	= THIS_MODULE,
	.write	= foo_write,
	.read	= foo_read
};
