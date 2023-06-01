#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/debugfs.h>

MODULE_AUTHOR("Wander Lairson Costa");
MODULE_DESCRIPTION("Test the CONFIG_PROVE_LOCKING_NESTED configuration");
MODULE_LICENSE("GPL");

static ssize_t trigger_splat(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	static DEFINE_SPINLOCK(slock);
	static DEFINE_RAW_SPINLOCK(raw_slock);

	raw_spin_lock(&raw_slock);
	spin_lock(&slock);

	spin_unlock(&slock);
	raw_spin_unlock(&raw_slock);

	return count;
}

static struct file_operations fops = {
	.write = trigger_splat,
};

static struct dentry *root;

static int provelock_init(void)
{
	struct dentry *splat_file;

	root = debugfs_create_dir(KBUILD_MODNAME, NULL);
	if (IS_ERR(root))
		return PTR_ERR(root);

	splat_file = debugfs_create_file(KBUILD_MODNAME, 0222,
				 root, NULL, &fops);

	if (IS_ERR(splat_file))
		return PTR_ERR(splat_file);

	return 0;
}

static void provelock_exit(void)
{
	debugfs_remove_recursive(root);
}

module_init(provelock_init);
module_exit(provelock_exit);
