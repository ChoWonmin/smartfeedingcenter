#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>

#define GPIO1 17
#define DEV_NAME "btn_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");

int btn_scan(struct inode *pinode, struct file *pfile){
    
    int res = -1;
    printk(KERN_ALERT "open btn_dev\n");
    
    gpio_request(GPIO1, "GPIO1");
    
    gpio_direction_input(GPIO1);
    
    while(1) {
        res = gpio_get_value(GPIO1);
        
        if(res){
            break;
        }
        
    }
    
    return 0;
}
struct file_operations fop = {
    .owner = THIS_MODULE,
    .open = btn_scan,
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

