#include <linux/fs.h>
#include <linux/cdev.h>

#ifndef __SCULL_H__
#define __SCULL_H__

struct scull_qset {
	void **data;
	struct scull_qset *next;
};

struct scull_dev {
	struct scull_qset *data;
	int quantum;
	int qset;
	unsigned long size;
	unsigned int access_key;
	struct semaphore sem;
	struct cdev cdev;
};

int scull_open(struct inode *inode, struct file *filp);
int scull_release(struct inode *inode, struct file *filp);
int scull_trim(struct scull_dev *dev);

struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	/* TODO: Fill these
	.llseek = scull_llseek,
	.read = scull_read,
	.write = scull_write,
	.ioctl = scull_ioctl,
	*/
	.open = scull_open,
	.release = scull_release,
};

#endif
