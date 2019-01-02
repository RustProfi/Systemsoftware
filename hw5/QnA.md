## Which functions are available in the kernel for allocating/freeing memory?
kmalloc, kcalloc(for arrays), kzalloc, krealloc, kmalloc_caches, kmalloc_index, kmem_cache_alloc, kfree, kzfree , kmem_cache_free, kmem_cache_free_bulk, kfree_bulk and some more.
## Which functions did you choose and why?
I chose the standard kmalloc, because for a simple counter (which is an int and could be on stack) and something i never need to be bigger or smaller, the simplest is fine. To free kmalloc i use kfree.
## Which is the more accurate time source: jiffies or CPU cycles?
## How can you identify the kernel thread's PID using the ps utility?
I can see the name of the thread in column command and simply look at the PID in column PID.
## Can your kernel thread receive and handle signals that were sent from userspace using kill?
## What is the name of the workqueue in the process list?
## How do the time measurements of the workqueue compare to the one from the timer?
## What problems can occur on module unload?
## How would the design of mutual exclusion look like using a spinclock?
## Which implementation would you prefer, and why?
## Can close and unload unconditionally clean up the resources?
