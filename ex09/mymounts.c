// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/mount.h>
#include <linux/nsproxy.h>
#include <linux/dcache.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/path.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("alevasse");
MODULE_DESCRIPTION("Get My Mounts!");

static int show_mount(struct mount *mnt, void *arg)
{
	struct seq_file *m = arg;
	struct path mnt_path = {
		.mnt = &mnt->mnt,
		.dentry = mnt->mnt.mnt_root
	};

	char *buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	char *path;

	if (!buf)
		return -ENOMEM;

	path = d_path(&mnt_path, buf, PAGE_SIZE);

	seq_printf(m, "Mount entry:\n");
	seq_printf(m, "  Devname: %s\n", mnt->mnt_devname);
	seq_printf(m, "  Path   : %s\n", path);
	seq_printf(m, "  Root dentry: %px\n", mnt->mnt.mnt_root);
	seq_printf(m, "  Mount addr : %px\n", mnt);

	kfree(buf);
	return 0;
}

static int mymounts_show(struct seq_file *m, void *v)
{
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;

	pr_info("Mount namespace: %px\n", ns);

	return iterate_mounts(show_mount, m, ns);
}

static int	mymounts_open(struct inode *inode, struct file *file)
{
	return single_open(file, mymounts_show, NULL);
}

static const struct file_operations	mymounts_fops = {
	.owner = THIS_MODULE,
	.open = mymounts_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release
};

static int	__init init_mymounts(void)
{
	proc_create("mymounts", 0444, NULL, &mymounts_fops);
	printk(KERN_INFO "Success: Proc device has been registered!\n");
	return 0;
}

static void      __exit clean(void)
{
	remove_proc_entry("mymounts", NULL);
        printk(KERN_INFO "Cleaning up mymounts module.\n");
}

module_init(init_mymounts);
module_exit(clean);
