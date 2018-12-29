#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h> /*contains alloc_chrdev_region*/
#include <linux/device.h> /*device create etc*/ 
#include <linux/kthread.h>
#include <linux/sched/signal.h> //kill_pid
#include <linux/wait.h>

static struct file_operations fops;
static dev_t template_dev_number;
static struct cdev * driver_object;
struct class *template_class;
//Thread wichitges zeugs
struct task_struct* kthreadId;
static wait_queue_head_t wq;
static DECLARE_COMPLETION(on_exit);

static int what_the_thread_does(void *);

static int __init ModInit(void)
{
    printk(KERN_ALERT "mykthreadinit\n");
    
    if(alloc_chrdev_region(&template_dev_number,0,1,"Mykthread") < 0)
        return -EIO;
    driver_object = cdev_alloc(); /*Anmldeobjekt reservieren */
    if(driver_object==NULL)
        goto free_device_number;
    driver_object->owner = THIS_MODULE;
    driver_object->ops = &fops;
    if(cdev_add(driver_object, template_dev_number, 1) < 0) //Treiber anmelden
        goto free_cdev;
    /*Eintrag im Sysfs, damit Udev den Geräteeintrag erzeugt */
    template_class = class_create(driver_object->owner, "mykthreaddrv");
    if(template_class == NULL) {
        class_destroy(template_class);
        goto free_cdev;
    }
    device_create(template_class, NULL, template_dev_number, NULL, "%s", "mykthread");
    
    init_waitqueue_head(&wq);
    //create thread
    kthreadId = kthread_create(what_the_thread_does, NULL, "mykthread");
    //and wake up
    wake_up_process(kthreadId);
    
    return 0;
    free_cdev:
    kobject_put(&driver_object->kobj );
    
    free_device_number:
    unregister_chrdev_region(template_dev_number, 1);
    return -EIO;
}

static void __exit ModExit(void)
{
    printk(KERN_ALERT "mykthread kill\n");
    
    //kill thread 
    //SIGTERM ist Signal 15
    //soweit ich das herausgefunden habe, entscheidet die 1 am Ende, dass SEND_SIG_PRIV macro getätigt wird. Bei 0 müsste keine Info kommen.
    kill_pid(task_pid(kthreadId), SIGTERM, 1);
    wait_for_completion(&on_exit);
    
    device_destroy(template_class, template_dev_number);
    class_destroy(template_class);
    /*Abmelden des Treibers*/
    cdev_del(driver_object);
    kobject_put(&driver_object->kobj);
    unregister_chrdev_region(template_dev_number, 1);
    return;
}

static int what_the_thread_does(void *data) {
    
    unsigned long timeout;
    allow_signal(SIGTERM);
    while(1) {
        printk(KERN_ALERT "A message XD\n");
        if(kthread_should_stop() == 1)
            break;
        timeout = HZ * 2;
        timeout = wait_event_interruptible_timeout(wq, false, timeout);
        
       if(timeout == -ERESTARTSYS)
           break;
    }
    printk(KERN_ALERT "EXTRA LOL\n");
    complete_and_exit(&on_exit, 0);
}

module_init(ModInit);
module_exit(ModExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marno Janetzky, Gabriel Cmiel");
MODULE_DESCRIPTION("Creates a kernel thread");
MODULE_SUPPORTED_DEVICE("mykthread");
