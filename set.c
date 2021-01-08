#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/delay.h>

MODULE_AUTHOR("Ryuichi Ueda and Inoue Satoshi");
MODULE_DESCRIPTION("driver for 4 LEDs control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;

static ssize_t led_write(struct file *filp, const char *buf, size_t count, loff_t *pos)
{
    char c;
    if(copy_from_user(&c,buf,sizeof(char)))
        return -EFAULT;
    int i = 0;
    int j = 0;
    printk(KERN_INFO "receive %c\n",c);
    int max = 0;
    if((c < '5') && ('0' < c)){
        max = c - '0';
    }
    else{
        max = 0;
    }
    for(i = 0;i < (1 << max);++i){
        for(j = 0;j < 4;++j){
            if( (i & (1 << j) )> 0){
                gpio_base[7] = 1 << (23+ j);
            }
            else{
                gpio_base[10] = 1 << (23 + j);
            }
        }
        mdelay(500);
    }
    return 1;
}

static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .write = led_write
};

static int __init init_mod(void)
{
    int retval;
    gpio_base = ioremap_nocache(0x3f200000, 0xA0);

    const u32 led_table[] = {23, 24, 25, 26};
    const u32 index = 2;
    const u32 shift_table[] = {(led_table[0]%10)*3,(led_table[1]%10)*3,(led_table[2]%10)*3,(led_table[3]%10)*3};
    const u32 mask = ~((0x07 << shift_table[0]) | (0x07 << shift_table[1]) | (0x07 << shift_table[2]) | (0x07 << shift_table[3]) );
    int id = 0;
    gpio_base[index] = (gpio_base[index] & mask);
    for(id = 0; id < 4;++id){
            gpio_base[index] = gpio_base[index] | (0x01 << shift_table[id]);
        }

    retval = alloc_chrdev_region(&dev, 0, 1, "set");
    if (retval < 0)
    {
        printk(KERN_ERR "alloc_chrdev_region failed.\n");
        return retval;
    }
    printk(KERN_INFO "%s is loaded. major:%d\n", __FILE__, MAJOR(dev));
    cdev_init(&cdv, &led_fops);
    retval = cdev_add(&cdv, dev, 1);
    if (retval < 0)
    {
        printk(KERN_ERR "cdev_add failed. major:%d, minor:%d", MAJOR(dev), MINOR(dev));
        return retval;
    }

    cls = class_create(THIS_MODULE, "set");
    if (IS_ERR(cls))
    {
        printk(KERN_ERR "class_create failed.");
        return PTR_ERR(cls);
    }
    device_create(cls, NULL, dev, NULL, "set%d", MINOR(dev));
    return 0;
}

static void __exit cleanup_mod(void)
{
    cdev_del(&cdv);
    device_destroy(cls, dev);
    class_destroy(cls);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "%s is unloaded. major:%d\n", __FILE__, MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);