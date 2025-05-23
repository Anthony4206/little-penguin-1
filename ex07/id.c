#include <linux/fs.h>
#include <linux/uaccess.h>

static char *login = "alevasse";
#define LOGIN_LEN 8

static ssize_t
id_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
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
		pr_info("Success: Right data received from user!\n");
		return count;	
	}
	printk(KERN_ERR "Error: Bad login name!\n");
	return -EINVAL;
}

static ssize_t
id_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	if (*ppos > 0) {
		printk(KERN_ERR "Error: EOF reached!\n");
		return 0;
	}
	if (count < LOGIN_LEN) {
		printk(KERN_ERR "Error: Count to small!\n");
		return -EINVAL;
	}
	if (copy_to_user(buf, login, LOGIN_LEN)) {
		printk(KERN_ERR "Error: Copy to user failed!\n");
		return -EFAULT;
	}
	*ppos += LOGIN_LEN;
	printk(KERN_INFO "Success: Login copied to user buffer!\n");
	return LOGIN_LEN;
}

const struct file_operations	s_fops_id = {
	.owner	= THIS_MODULE,
	.write	= id_write,
	.read	= id_read
};
