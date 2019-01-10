#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h> /*contains alloc_chrdev_region*/
#include <linux/device.h> /*device create etc*/ 
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/slab.h> //alloc
#include <linux/types.h>
#include <linux/semaphore.h>
#define EOF (-1)
#define BUFSIZE 32 * sizeof(char)

static dev_t template_dev_number;
static struct cdev * driver_object;
struct class *template_class;
int i;
char *buffer;
static wait_queue_head_t wqueue;
bool daten_da;
static DEFINE_SEMAPHORE(mysema);

static ssize_t driver_read(struct file *, char *, size_t, loff_t *) ;
static ssize_t driver_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
    .read = driver_read,
    .write = driver_write,
};

static int __init ModInit(void)
{
    printk(KERN_ALERT "mybuffer_sync init\n");
    if(alloc_chrdev_region(&template_dev_number,0,1,"MyBuffer_sync")<0)
        return -EIO;
    driver_object = cdev_alloc(); /*Anmldeobjekt reservieren */
    if(driver_object==NULL)
        goto free_device_number;
    driver_object->owner = THIS_MODULE;
    driver_object->ops = &fops;
    if(cdev_add(driver_object, template_dev_number, 1) < 0) //Treiber anmelden
        goto free_cdev;
    /*Eintrag im Sysfs, damit Udev den Geräteeintrag erzeugt */
    template_class = class_create(driver_object->owner,"mybuffer_sync_drv");
    if(template_class == NULL)
        goto free_cdev;
    
    if(device_create(template_class, NULL, template_dev_number, NULL, "%s", "mybuffer_sync") == NULL){
        class_destroy(template_class);
        goto free_cdev;
    }
    
    init_waitqueue_head(&wqueue);
    buffer = (char *) kmalloc(BUFSIZE, GFP_KERNEL);
    daten_da = false;
    return 0;
    
    free_cdev:
    kobject_put(&driver_object->kobj );
    
    free_device_number:
    unregister_chrdev_region(template_dev_number, 1);
    return -EIO;
}

static void __exit ModExit(void)
{
    printk(KERN_ALERT "mybuffer_sync  kill\n");
    device_destroy(template_class, template_dev_number);
    
    class_destroy(template_class);
    /*Abmelden des Treibers*/
    cdev_del(driver_object);
    kobject_put(&driver_object->kobj);
    unregister_chrdev_region(template_dev_number, 1);
    kfree(buffer);
    daten_da = false;
    return;
}

static ssize_t driver_read(struct file *instanz, char *user, size_t count, loff_t *offset) {
    
    int not_copied, to_copy;
    
    if(wait_event_interruptible(wqueue, daten_da)){
        return -ERESTARTSYS;
    }
    to_copy = min(BUFSIZE, count);
    not_copied = copy_to_user(user, buffer, to_copy);
    up(&mysema);
    return to_copy - not_copied;
}

static ssize_t driver_write(struct file *instanz, const char *userbuf, size_t count, loff_t *offs) {
    int to_copy, not_copied;
    
    if(count == 0)
        return 0;
    
    to_copy = min( count, BUFSIZE);
    
    while(1) {
        if(down_timeout(&mysema, HZ) == 0) {
            not_copied = copy_from_user(buffer, userbuf, to_copy);
            break;
        }
    }
    
    if(to_copy-not_copied > 0) {
        daten_da = true;
        wake_up_interruptible(&wqueue);
    }
    return to_copy-not_copied;
}

module_init(ModInit);
module_exit(ModExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marno Janetzky, Gabriel Cmiel");
MODULE_DESCRIPTION("Mybuffer_sync provides a 32 Byte Buffer which is thread safe.");
MODULE_SUPPORTED_DEVICE("mybuffer_sync");
