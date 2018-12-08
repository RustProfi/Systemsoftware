#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h> /*contains alloc_chrdev_region and MKDEV macro*/
#include <linux/device.h> /*device create etc*/ 
#include <linux/slab.h> //kmalloc, kfree


static int driver_open(struct inode *, struct file *);
static int driver_close(struct inode *, struct file *);

static struct file_operations fops = 
{
    .open = driver_open,
    .release = driver_close,
};

static dev_t template_dev_number;
//dev_t deviceNumbers[257];
dev_t *deviceNumbers;
static struct cdev * driver_object;
struct class *template_class;
int major, i;
dev_t devNr;
static atomic_t acounter = ATOMIC_INIT(0);

//returned die device number. N ist gleich der Minor Nr
dev_t createDevice(int majorNr, int N) {
    devNr = MKDEV(majorNr, N);
    
    device_create(template_class, NULL, devNr, NULL, "openclose%d",  N);
    return devNr;
}

static int __init ModInit(void)
{
    printk(KERN_ALERT "open close init\n");
    
    //allokiert für 257 devices
    if(alloc_chrdev_region(&template_dev_number,0,257,"OpenCloseD") < 0)
        return -EIO;
    driver_object = cdev_alloc(); /*Anmldeobjekt reservieren */
    if(driver_object==NULL)
        goto free_device_number;
    driver_object->owner = THIS_MODULE;
    driver_object->ops = &fops;
    //257 minor numbers
    if(cdev_add(driver_object, template_dev_number, 257) < 0) //Treiber anmelden
        goto free_cdev;
    /*Eintrag im Sysfs, damit Udev den Geräteeintrag erzeugt */
    template_class = class_create(driver_object->owner, "openclosedrv");
    if(template_class == NULL)
        goto free_cdev;
        
    deviceNumbers = (dev_t *) kmalloc(257*sizeof(dev_t), GFP_KERNEL);
    
    for( i = 0; i < 257; i++) {
        deviceNumbers[i] = createDevice(MAJOR(template_dev_number), i);
    }
    
    return 0;
    
    free_cdev:
    kobject_put(&driver_object->kobj );
    
    free_device_number:
    unregister_chrdev_region(template_dev_number, 257);
    return -EIO;
}

static void __exit ModExit(void)
{
    printk(KERN_ALERT "open close kill\n");
    
    //Destroy devices
    for(i = 0; i < 257; i++) {
        device_destroy(template_class, deviceNumbers[i]);
    }
    
    class_destroy(template_class);
    /*Abmelden des Treibers*/
    cdev_del(driver_object);
    kobject_put(&driver_object->kobj);
    unregister_chrdev_region(template_dev_number, 257);
    
    kfree(deviceNumbers);
    return;
    
}

static int driver_open(struct inode *geraet, struct file *instanz) {
    if(instanz->f_flags&O_RDWR || instanz->f_flags&O_WRONLY) {
        if (atomic_read(&acounter) >= MINOR(geraet->i_rdev)) {
            return -EBUSY;
        }
        atomic_inc(&acounter);
    }
    return 0;
}

static int driver_close(struct inode *geraet, struct file *instanz) {
    if(instanz->f_flags&O_RDWR || instanz->f_flags&O_WRONLY) {
        atomic_dec(&acounter);
    }
    return 0;
}


module_init(ModInit);
module_exit(ModExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marno Janetzky, Gabriel Cmiel");
MODULE_DESCRIPTION("Simple Chardev");
MODULE_SUPPORTED_DEVICE("openclose");
