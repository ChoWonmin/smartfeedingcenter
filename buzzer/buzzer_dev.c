#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>

#define GPIO1 18
#define DEV_NAME "buzzer_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");

int buzzer_open(struct inode *pinode, struct file *pfile) {
	// At first, buzzer is initialized that it is shut down.
	printk(KERN_ALERT "OPEN buzzer_dev\n");
	gpio_request(GPIO1, "GPIO1");
	gpio_direction_output(GPIO1, 1);
	return 0;
}

int buzzer_close(struct inode *pinode, struct file *pfile) {
	printk(KERN_ALERT "RELEASE buzzer_dev\n");

	return 0;
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = buzzer_open,
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
