#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/version.h>

static void inc_count(struct work_struct *work);
static DECLARE_DELAYED_WORK(work_obj, inc_count);
static struct workqueue_struct *wq;

static unsigned long currj, prevj, maxj, minj;
static cycles_t currc, prevc, maxc, minc;
static u64 currn, prevn, maxn, minn;

static void inc_count(struct work_struct *work)
{
	currj = jiffies - prevj;
	maxj = max(currj,maxj);
	minj = min(currj,minj);
	prevj = jiffies;

	currc = get_cycles() - prevc;
	maxc = max(currc,maxc);
	minc = min(currc,minc);
	prevc = get_cycles();

	currn = jiffies_to_nsecs(jiffies) - prevn;
	maxn = max(currn,maxn);
	minn = min(currn,minn);
	prevn = jiffies_to_nsecs(jiffies);

	printk("Jiffies2: (%ld), current: (%ld), min: (%ld), max: (%ld)",jiffies, currj, minj, maxj);
	printk("CPU Cycles2: (%llu), current: (%lu), min: (%lu), max: (%lu)",get_cycles(), currc, minc, maxc);
	printk("Nanoseconds2: (%llu), current: (%llu), min: (%llu), max: (%llu)",jiffies_to_nsecs(jiffies), currn, minn, maxn);

	if(queue_delayed_work( wq, &work_obj, 2*HZ) ) {
		pr_debug( "queue_work successful ...\n");
	} else {
		pr_debug( "queue_work not successful ...\n");
	}
	return;
}


static int __init drv_init(void)
{
	// create Workqueue
	minj = jiffies;
	currj = jiffies;
	prevj  = jiffies;
	maxj = 0;

	minc = get_cycles();
	currc = get_cycles();
	prevc  = get_cycles();
	maxc = 0;

	minn = jiffies_to_nsecs(jiffies);
	currn = jiffies_to_nsecs(jiffies);
	prevn  = jiffies_to_nsecs(jiffies);

	wq = create_singlethread_workqueue("MyWorkqueue");
	if(queue_delayed_work( wq, &work_obj, 2*HZ) ) {
		pr_debug( "queue_work successful ...\n");
	} else {
		pr_debug( "queue_work not successful ...\n");
	}
	return 0;
}

static void __exit drv_exit(void)
{
	pr_debug("drv_exit called\n");
	if(wq){
		cancel_delayed_work(&work_obj);
		flush_workqueue(wq);
		destroy_workqueue(wq);
		pr_debug("Workqueue destroyed\n");
		printk("Workqueue deaktiviert\n");
	}

}

module_init(drv_init);
module_exit(drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marno Janetzky, Gabriel Cmiel");
MODULE_DESCRIPTION("myworkqueue");
