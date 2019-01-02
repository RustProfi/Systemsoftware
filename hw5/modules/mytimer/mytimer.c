#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>

static struct timer_list jifftime;
//static struct timer_list cpu_cycles;
//static struct timer_list nanoseconds;


static void inc_count(unsigned long arg)
{
	printk("current inter-trigger duration (%ld)\n", jifftime.expires);
	//printk("minimum inter-trigger duration (%ld)\n")
	//printk("maximum inter-trigger duration ")
}

static int __init ktimer_init(void)
{
	init_timer(&jifftime);
	timer_list jifftime =  {
	.function = inc_count,
	.data = 0,
	.expires = jiffies + (2*HZ)
	};
	//jifftime.function = inc_count;
	//jifftime.data = 0;
	//jifftime.expires = jiffies + (2*HZ);
	//init_timer(&cpu_cycles);
	//init_timer(&nanoseconds);

	//mytimer.function = inc_count;
	//mytimer.data = 0;
	//mytimer.expires = jiffies + (2*HZ);
	add_timer(&jifftime);
	return 0;
}

static void __exit ktimer_exit(void)
{
	if(del_timer_sync(&jifftime)){
		printk("Aktiver Timer deaktiviert\n");
	}
}

module_init(ktimer_init);
module_exit(ktimer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marno Janetzky, Gabriel Cmiel");
MODULE_DESCRIPTION("Timer");
