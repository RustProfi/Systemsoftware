#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int __init ModInit(void)
{
        printk(KERN_ALERT "Hello, kworld\n");
        return 0;
}

static void __exit ModExit(void)
{
        printk(KERN_ALERT "Goodbye, cruel kworld\n");
}

module_init(ModInit);
module_exit(ModExit);