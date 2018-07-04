#include <linux/init.h>
#include <linux/module.h>

#include "scull.h"

MODULE_LICENSE("Dual BSD/GPL");

int scull_major = 0; // TODO: Do this
int scull_minor = 0; // TODO: Do this
struct scull_dev main_cdev;
int scull_quantum = 1000; // TODO: What is this size?
int scull_qset = 1000; // TODO: What is this size?

static void scull_setup_cdev(struct scull_dev *dev, int index) {
	int err, devno = MKDEV(scull_major, scull_minor + index);

	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;
	err = cdev_add(&dev->cdev, devno, 1);
	if (err) {
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
	}
}

static int scull_init(void) {
	printk(KERN_ALERT "Hello, world!\n");
	scull_setup_cdev(&main_cdev, 0);
	return 0;
}

static void scull_exit(void) {
	printk(KERN_ALERT "Goodbye, cruel world!\n");
}

int scull_open(struct inode *inode, struct file *filp) {
	// TODO: Fill this in
	return 0;
}

int scull_release(struct inode *inode, struct file *filp) {
	return 0;
}

int scull_trim(struct scull_dev *dev) {
	struct scull_qset *next, *dptr;
	int qset = dev->qset;
	int i;
	for (dptr = dev->data; dptr; dptr = next) {
		if (dptr->data) {
			for (i = 0; i < qset; i++) {
				kfree(dptr->data[i]);
			}
			kfree(dptr->data);
			dptr->data = NULL;
		}
		next = dptr->next;
		kfree(dptr);
	}
	dev->size = 0;
	dev->quantum = scull_quantum;
	dev->qset = scull_qset;
	dev->data = NULL;

	return 0;
}

module_init(scull_init);
module_exit(scull_exit);
