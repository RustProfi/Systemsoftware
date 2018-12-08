#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h> /*contains alloc_chrdev_region*/
#include <linux/device.h> /*device create etc*/ 
#include <asm/errno.h>

static struct file_operations fops;
static dev_t template_dev_number;
static struct cdev * driver_object;
struct class *template_class;
static int write_count=0;

static int __init ModInit(void)
{
    printk(KERN_ALERT "OpenClose init\n");
    
    if(alloc_chrdev_region(&template_dev_number,0,256,"OpenClose") < 0)
        return -EIO;
    driver_object = cdev_alloc(); /*Anmldeobjekt reservieren */
    if(driver_object==NULL)
        goto free_device_number;
    driver_object->owner = THIS_MODULE;
    driver_object->ops = &fops;
    if(cdev_add(driver_object, template_dev_number, 1) < 0) //Treiber anmelden
        goto free_cdev;
    /*Eintrag im Sysfs, damit Udev den Geräteeintrag erzeugt */
    template_class = class_create(driver_object->owner, "chardrv");
    if(template_class == NULL) {
	class_destroy(template_class);
	goto free_cdev;
    }
    device_create(template_class, NULL, template_dev_number, NULL, "%s", "simple_chardrv");
    
    return 0;
    free_cdev:
    kobject_put(&driver_object->kobj );
    
    free_device_number:
    unregister_chrdev_region(template_dev_number, 1);
    return -EIO;
}

static void __exit ModExit(void)
{
    printk(KERN_ALERT "char dev kill\n");
    device_destroy(template_class, template_dev_number);
    class_destroy(template_class);
    /*Abmelden des Treibers*/
    cdev_del(driver_object);
    kobject_put(&driver_object->kobj);
    unregister_chrdev_region(template_dev_number, 1);
    return;
    
}

static int driver_open(struct inode *geraetedatei, struct file *instanz){
	if(instanz->f_flags&O_RDWR || instanz->f_flags&O_WRONLY) {
		if(write_count > 0) {
			return -EBUSY;
		}
		write_count++;
	}
	return 0;
}

static int driver_close(struct inode *geraetedatei, struct file *instanz) {
	if(instanz->f_flags&O_RDWR || instanz->f_flags&O_WRONLY) {
		write_count--;
	}
	return 0;
}


module_init(ModInit);
module_exit(ModExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marno Janetzky, Gabriel Cmiel");
MODULE_DESCRIPTION("OpenClose");
MODULE_SUPPORTED_DEVICE("openclose");
