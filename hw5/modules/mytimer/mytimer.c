#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>

static struct timer_list mytimer;
static unsigned long curr, prev, max, min;
//static unsigned long max = 0;
static void inc_count(unsigned long arg)
{
	curr = jiffies - prev;
	max = max(curr,max);
	min = min(curr,min);
	prev = jiffies;
	printk("Jiffies: (%ld), current: %lu, min: %lu, max: %lu",mytimer.expires, curr, min, max);
	mod_timer(&mytimer,jiffies + (2*HZ));
	//mytimer.expires =  jiffies + (2*HZ);
	//add_timer(&mytimer);
}

static int __init ktimer_init(void)
{
  init_timer(&mytimer);
	mytimer.function = inc_count;
	mytimer.data = 0;
	mytimer.expires =  jiffies + (2*HZ);
	min = jiffies;
	curr = jiffies;
	prev  = jiffies;
	max = 0;
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
