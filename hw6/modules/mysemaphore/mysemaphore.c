#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h> /*contains alloc_chrdev_region*/
#include <linux/device.h> /*device create etc*/ 
#include <linux/semaphore.h>

static dev_t template_dev_number;
static struct cdev * driver_object;
struct class *template_class;
static DEFINE_SEMAPHORE(mysema);
static wait_queue_head_t wq;

static int driver_open(struct inode *, struct file *);
static void semaroutine(void);

static struct file_operations fops = {
    .open = driver_open,
};

static int __init ModInit(void)
{
    printk(KERN_ALERT "mysemaphore init\n");
    if(alloc_chrdev_region(&template_dev_number,0,1,"mysemaphore")<0)
        return -EIO;
    driver_object = cdev_alloc(); /*Anmldeobjekt reservieren */
    if(driver_object==NULL)
        goto free_device_number;
    driver_object->owner = THIS_MODULE;
    driver_object->ops = &fops;
    if(cdev_add(driver_object, template_dev_number, 1) < 0) //Treiber anmelden
        goto free_cdev;
    /*Eintrag im Sysfs, damit Udev den Geräteeintrag erzeugt */
    template_class = class_create(driver_object->owner,"mysemaphoredrv");
    if(template_class == NULL)
        goto free_cdev;
    
    if(device_create(template_class, NULL, template_dev_number, NULL, "%s", "mysemaphore") == NULL){
        class_destroy(template_class);
        goto free_cdev;
    }
    
    init_waitqueue_head(&wq);
    return 0;
    
    free_cdev:
    kobject_put(&driver_object->kobj );
    
    free_device_number:
    unregister_chrdev_region(template_dev_number, 1);
    return -EIO;
}

static void __exit ModExit(void)
{
    printk(KERN_ALERT "mysemaphore kill\n");
    device_destroy(template_class, template_dev_number);
    
    class_destroy(template_class);
    /*Abmelden des Treibers*/
    cdev_del(driver_object);
    kobject_put(&driver_object->kobj);
    unregister_chrdev_region(template_dev_number, 1);
    return;
}

static int driver_open(struct inode *geraetedatei, struct file *instanz){
    if(down_trylock(&mysema) == 0) {
        semaroutine();
        return 0;
    }
    else {
        printk(KERN_ALERT "critical section is busy :(\n");
        while(1) {
            if(down_timeout(&mysema, HZ/5) == 0) {
                printk(KERN_ALERT "Finally :)\n");
                semaroutine();
                return 0;
            }
        }
    }
}

static void semaroutine() {
    unsigned long timeout = HZ *3;
    wait_event_interruptible_timeout(wq, timeout == 0, timeout);
    up(&mysema);
}


module_init(ModInit);
module_exit(ModExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marno Janetzky, Gabriel Cmiel");
MODULE_DESCRIPTION("mysemaphore implements a semaphore");
MODULE_SUPPORTED_DEVICE("mysemaphore");
