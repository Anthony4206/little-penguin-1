#include <linux/module.h>
#include <linux/debugfs.h>

MODULE_AUTHOR("alevasse");
MODULE_DESCRIPTION("Debugfs user/kernel interface!");
MODULE_LICENSE("GPL");

extern const struct file_operations s_fops_id;
extern const struct file_operations s_fops_jiffies;
extern const struct file_operations s_fops_foo;

static struct dentry	*dir;

static int	__init debugfs_init(void)
{
	static struct dentry     *file_id;
	static struct dentry     *file_jiffies;
	static struct dentry     *file_foo;

	dir = debugfs_create_dir("fortytwo", NULL);
	if (!dir) {
		printk(KERN_ERR "debugfs not available or failed!\n");
		return -ENODEV;
	}
	printk(KERN_INFO "Success: Debugfs device has been registered!\n");
	
	file_id = debugfs_create_file("id", 0666, dir, NULL, &s_fops_id);
	if (!file_id)
		goto fail;
	
	file_jiffies = debugfs_create_file("jiffies", 0444, dir,
					   NULL, &s_fops_jiffies);
	if (!file_jiffies)
		goto fail;
	
	file_foo = debugfs_create_file("foo", 0644, dir, NULL, &s_fops_foo);
	if (!file_foo)
		goto fail;
	printk(KERN_INFO "Success: All files have been created!\n");
	return 0;

fail:
	debugfs_remove_recursive(dir);
	printk(KERN_ERR "A debugfs_create_file failed!\n");
	return -1;
}

static void      __exit debugfs_exit(void)
{
        debugfs_remove_recursive(dir);
        printk(KERN_INFO "Debugfs device has been deregistered!\n");
}

module_init(debugfs_init);
module_exit(debugfs_exit);
