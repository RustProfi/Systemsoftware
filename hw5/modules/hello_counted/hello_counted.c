#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h> /*contains alloc_chrdev_region*/
#include <linux/device.h> /*device create etc*/ 
#include <asm/uaccess.h> /*copy to user*/
#include <linux/slab.h> //kmalloc and kfree

static dev_t template_dev_number;
static struct cdev * driver_object;
struct class *template_class;
int* hellocounter;

static int driver_open(struct inode *, struct file *);
static int driver_close(struct inode *, struct file *); 
static ssize_t driver_read(struct file *, char *, size_t, loff_t *) ;

static struct file_operations fops = {
    .open = driver_open,
    .release = driver_close,
    .read = driver_read,
};


static int __init ModInit(void)
{
    printk(KERN_ALERT "hello counted init\n");
    if(alloc_chrdev_region(&template_dev_number,0,1,"Hello counted")<0)
        return -EIO;
    driver_object = cdev_alloc(); /*Anmldeobjekt reservieren */
    if(driver_object==NULL)
        goto free_device_number;
    driver_object->owner = THIS_MODULE;
    driver_object->ops = &fops;
    if(cdev_add(driver_object, template_dev_number, 1) < 0) //Treiber anmelden
        goto free_cdev;
    /*Eintrag im Sysfs, damit Udev den Geräteeintrag erzeugt */
    template_class = class_create(driver_object->owner,"hellocounteddrv");
    if(template_class == NULL)
        goto free_cdev;
    
    if(device_create(template_class, NULL, template_dev_number, NULL, "%s",  "hello_counted") == NULL){
        class_destroy(template_class);
        goto free_cdev;
    }
    
    return 0;
    
    free_cdev:
    kobject_put(&driver_object->kobj );
    
    free_device_number:
    unregister_chrdev_region(template_dev_number, 1);
    return -EIO;
}

static void __exit ModExit(void)
{
    printk(KERN_ALERT "hello counted kill\n");
    device_destroy(template_class, template_dev_number);
    
    class_destroy(template_class);
    /*Abmelden des Treibers*/
    cdev_del(driver_object);
    kobject_put(&driver_object->kobj);
    unregister_chrdev_region(template_dev_number, 1);
    return;
}

static int driver_open(struct inode *geraetedatei, struct file *instanz){
    if(instanz->f_flags&O_RDWR || instanz->f_flags&O_RDONLY) { 
        hellocounter = (int *) kmalloc(1 * sizeof(int), GFP_KERNEL);
        *hellocounter = 0;
        return 0;
    }
    else {
        return -1;
    }
}

static int driver_close(struct inode *geraetedatei, struct file *instanz) {
    if(instanz->f_flags&O_RDWR || instanz->f_flags&O_RDONLY) { 
        kfree(hellocounter);
        return 0;
    }
    else {
        return -1;
    }
}

static ssize_t driver_read(struct file *instanz, char *user, size_t count, loff_t *offset) {
    int not_copied, to_copy, wrotebytes;
    char msg[100];

    snprintf(msg, 100, "Hello, from grp5. You have read %d bytes so far.", *hellocounter);

    to_copy = strlen(msg)+1;
    to_copy = min(to_copy,(int) count);
    not_copied = copy_to_user(user, msg, to_copy);
    wrotebytes = to_copy - not_copied;
    *hellocounter += wrotebytes;
    return wrotebytes;
}

module_init(ModInit);
module_exit(ModExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marno Janetzky, Gabriel Cmiel");
MODULE_DESCRIPTION("Hello counted");
MODULE_SUPPORTED_DEVICE("hello_counted");
