#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define GPIO1 17
#define DEV_NAME "btn_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");
static char *msg = "";
int res = 0;

int btn_open(struct inode *pinode, struct file *pfile){
    
    printk(KERN_ALERT "open btn_dev\n");
    
    gpio_request(GPIO1, "GPIO1");
    gpio_direction_input(GPIO1);
    
    return 0;
}

ssize_t btn_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
    res = gpio_get_value(GPIO1);
    
    if(!res)
        copy_to_user(buffer, "click", length);
    else
        copy_to_user(buffer, "no click", length);

    return 0;
}
ssize_t btn_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
    printk(KERN_ALERT "Write\n");
    
    if(copy_from_user(msg, buffer, length)) {
        printk("write error\n");
        return -1;
    }
    
    return length;
}
struct file_operations fop = {
    .owner = THIS_MODULE,
    .open = btn_open,
    .read = btn_read,
    .write = btn_write,
};

int __init btn_init(void) {
    printk(KERN_ALERT "init btn get value\n");
    register_chrdev(DEV_NUM, DEV_NAME, &fop);
    return 0;
};
void __exit btn_exit(void) {
    printk(KERN_ALERT "exit btn_dev\n");
    unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(btn_init);
module_exit(btn_exit);


