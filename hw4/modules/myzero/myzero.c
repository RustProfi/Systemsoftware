#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h> /*contains alloc_chrdev_region*/
#include <linux/device.h> /*device create etc*/ 
#include <asm/uaccess.h>
#include <linux/random.h>
#include <linux/slab.h> //kmalloc kfree

static dev_t template_dev_number;
static struct cdev * driver_object;
struct class *template_class;
int i;

static ssize_t driver_write(struct file *, const char *, size_t, loff_t *);
static ssize_t driver_read(struct file *, char *, size_t, loff_t *) ;
int getrnd(void);

static struct file_operations fops = {
    .read = driver_read,
    .write = driver_write,
};


static int __init ModInit(void)
{
    printk(KERN_ALERT "myzero init\n");
    if(alloc_chrdev_region(&template_dev_number,0,1,"MyZero")<0)
        return -EIO;
    driver_object = cdev_alloc(); /*Anmldeobjekt reservieren */
    if(driver_object==NULL)
        goto free_device_number;
    driver_object->owner = THIS_MODULE;
    driver_object->ops = &fops;
    if(cdev_add(driver_object, template_dev_number, 1) < 0) //Treiber anmelden
        goto free_cdev;
    /*Eintrag im Sysfs, damit Udev den Geräteeintrag erzeugt */
    template_class = class_create(driver_object->owner,"myzerodrv");
    if(template_class == NULL)
        goto free_cdev;
    
    if(device_create(template_class, NULL, template_dev_number, NULL, "%s", "myzero") == NULL){
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
    printk(KERN_ALERT "myzero  kill\n");
    device_destroy(template_class, template_dev_number);
    
    class_destroy(template_class);
    /*Abmelden des Treibers*/
    cdev_del(driver_object);
    kobject_put(&driver_object->kobj);
    unregister_chrdev_region(template_dev_number, 1);
    return;
}

int getrnd() {
    unsigned int i;
    get_random_bytes(&i, sizeof(unsigned int));
    return i < 0 ? (i *-1) % 50000 : i % 50000;
}

static ssize_t driver_read(struct file *instanz, char *user, size_t count, loff_t *offset) {
    int rnd, not_copied, to_copy;
    char *buf;
    rnd = getrnd();

    buf = (char *) kzalloc(rnd*sizeof(char), GFP_KERNEL);

    to_copy = strlen(buf);
    to_copy = min(to_copy,(int) count);
    not_copied = copy_to_user(user, buf, to_copy);
    kfree(buf);
    return to_copy - not_copied;

}

static ssize_t driver_write(struct file *instanz, const char *userbuf, size_t count, loff_t *offs) {
    return count;
}


module_init(ModInit);
module_exit(ModExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marno Janetzky, Gabriel Cmiel");
MODULE_DESCRIPTION("Myzero");
MODULE_SUPPORTED_DEVICE("myzero");
