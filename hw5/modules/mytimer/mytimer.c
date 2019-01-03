#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>

static struct timer_list mytimer;
static unsigned long currj, prevj, maxj, minj;
static cycles_t currc, prevc, maxc, minc;
static u64 currn, prevn, maxn, minn;
//static unsigned long max = 0;
static void inc_count(unsigned long arg)
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
	printk("Jiffies: (%ld), current: (%ld), min: (%ld), max: (%ld)",mytimer.expires, currj, minj, maxj);
	printk("CPU Cycles: (%llu), current: (%lu), min: (%lu), max: (%lu)",get_cycles(), currc, minc, maxc);
	printk("Nanoseconds: (%llu), current: (%llu), min: (%llu), max: (%llu)",jiffies_to_nsecs(jiffies), currn, minn, maxn);
	mod_timer(&mytimer,jiffies + (2*HZ));
}

static int __init ktimer_init(void)
{
  init_timer(&mytimer);
	mytimer.function = inc_count;
	mytimer.data = 0;
	mytimer.expires =  jiffies + (2*HZ);
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
	maxn = 0;
	add_timer(&mytimer);
	return 0;
}

static void __exit ktimer_exit(void)
{
	if(del_timer_sync(&mytimer)){
		printk("Aktiver Timer deaktiviert\n");
	}
}

module_init(ktimer_init);
module_exit(ktimer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marno Janetzky, Gabriel Cmiel");
MODULE_DESCRIPTION("Timer");
