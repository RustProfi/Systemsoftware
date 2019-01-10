#include <linux/version.h>
#include <linux/module.h>
#include <linux/interrupt.h>

// Tasklet Funktion
static void tasklet_function(unsigned long data)
{
	printk("Tasklet called...\n");
	return;
}
// Tasklet anlegen
DECLARE_TASKLET(tldescr, tasklet_function,0L);

static int __init ModInit(void)
{
	printk("Tasklet grp5\n");
	// Tasklet nach Abarbeitung  des nächsten Interrupts aufgerufen
	tasklet_schedule(&tldescr);
	return 0;
}

static void __exit ModExit(void)
{
	printk("mod_exit called\n");
	//Tasklet entfernen
	tasklet_kill(&tldescr);
}

module_init(ModInit);
module_exit(ModExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marno Janetzky, Gabriel Cmiel");
MODULE_DESCRIPTION("mytasklet");
