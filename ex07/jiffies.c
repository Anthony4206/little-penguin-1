#include <linux/fs.h>
#include <linux/jiffies.h>

static ssize_t
jiffies_read(struct file *file, char __user *u_buf, size_t count, loff_t *ppos)
{
	char	buf[64];
	int	len;
	u64	jiffies;

	jiffies = get_jiffies_64();
	len = snprintf(buf, sizeof(buf), "%llu\n", jiffies);
	printk(KERN_INFO "Jiffies has been called!\n");
	return simple_read_from_buffer(u_buf, count, ppos, buf, len);
}

const struct file_operations   s_fops_jiffies = {
        .owner  = THIS_MODULE,
        .read   = jiffies_read
};
