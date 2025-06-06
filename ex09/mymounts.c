// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <../fs/mount.h>
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

static void mymounts_format_line(struct seq_file *m,
				 struct mount *mnt,
				 struct path *mnt_path,
				 char *buf)
{
	const char *devname = (mnt->mnt_devname && mnt->mnt_devname[0] != '\0')
				? mnt->mnt_devname
				: "unknown";

	char *path = d_path(mnt_path, buf, PAGE_SIZE);
	if (IS_ERR(path))
		return;

	if (strlen(devname) >= 8)
		seq_printf(m, "%s\t%s\n", devname, path);
	else
		seq_printf(m, "%s\t\t%s\n", devname, path);
}

static void mymounts_print_entry(struct seq_file *m, struct mount *mnt)
{
	struct vfsmount *vfsmnt = &mnt->mnt;
	struct dentry *root = vfsmnt->mnt_root;
	struct path mnt_path = {
		.mnt = vfsmnt,
		.dentry = root
	};

	char *buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!buf)
		return;

	mymounts_format_line(m, mnt, &mnt_path, buf);

	kfree(buf);
}

static int mymounts_show(struct seq_file *m, void *v)
{
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	struct mount *mnt;

	list_for_each_entry(mnt, &ns->list, mnt_list)
		mymounts_print_entry(m, mnt);

	return 0;
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
