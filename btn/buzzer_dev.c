#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/timer.h>

#define GPIO1 18
#define DEV_NAME "buzzer_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");

int buzzer_open(struct inode *pinode, struct file *pfile) {
	// At first, buzzer is initialized that it is shut down.
	printk(KERN_ALERT "OPEN buzzer_dev\n");
	gpio_request(GPIO1, "GPIO1");
	gpio_direction_output(GPIO1, 0);
	return 0;
}

int buzzer_close(struct inode *pinode, struct file *pfile) {
	printk(KERN_ALERT "RELEASE buzzer_dev\n");

	return 0;
}

ssize_t buzzer_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	char msg[5];
	int err;
	// receive message from user program
	if((err = copy_from_user(msg, buffer, length)) < 0)
	{
		printk(KERN_ALERT "failed to write\n");
		return -1;
	}

	if(strcmp(msg, "buzz") == 0)
	{
		gpio_direction_output(GPIO1, 1);
		ssleep(1);
		gpio_direction_output(GPIO1, 0);
		ssleep(1);
	}

	return length;
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = buzzer_open,
	.write = buzzer_write,
	.release = buzzer_close,
};

int __init buzzer_init(void) {
	printk(KERN_ALERT "INIT buzzer\n");
	register_chrdev(DEV_NUM, DEV_NAME, &fop);
	return 0;
}

void __exit buzzer_exit(void) {
	printk(KERN_ALERT "EXIT buzzer_dev\n");
	unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(buzzer_init);
module_exit(buzzer_exit);
