## What do the both mentioned commandline options do?
"set architecture aarch64" sets current target architecture
"target remote tcp:127.0.0.1:1234" connects gdb to qemu through qemu's gdb server on port 1234 
## What do the Commands in gdb do?
"b start_kernel" sets a breakpoint at the symbol start_kernel
"c" continues the original programm
## What is a breakpoint?
A breakpoint makes your program stop whenever a certain point in the program is reached
## Which registers are displayed by qemu? which by gdb?(Shorten the answers by summing things together)
Both:
* x0-x30
* PC
* SP
* FPCR
* FPSR
Qemu:
* PSTATE
* q00-q31
GDB:
* cpsr
## When info is entered in qemu you can use tab-Completion to find more information objects. which other Information objects are there?
?
acl_policy
acl_reset
ballon
block_job_complete
bloc_job_resume
block_passwd
block_set-io_throtle
boot_set
change
chardev-remove
closefd
cont
cpu-add
device_ad
drive_add
drive_del
dump-guest-memory
expire_password
getfd
host_net_add
hostfwd_add
i
loadvm
logfile
migrate
migrate_incoming
migrate_set_capability
migrate_set_parameter
migrate_start_postcopy
mouse_move
nbd_server_add
nbd_server_stopp
netdev_del
o
object_del
pcie_aer_inject_error
print
qemu-io
qom-set
ringbuf_read
savevm
sendkey
set_password
snapshot_blkdev
snapshot_delete_blkdev_interal
stopcapture
system_powerdown
system_wakeup
usb_add
watchdog_action
x
xp
acl_add
acl_remove
acl_show
block_job_cancel
block_job_pause
block_job_set_speed
block_resize
block_stream
c
chardev-add
client_migrate_info
commit
cpu
delvm
device_del
drive_backup
drive_mirror
eject
gdbserver
help
host_net_remove
hostfwd_remove
info
log
memsave
migrate_cancel
migrate_set_cache_size
migrate_set_downtime
migrate_set_speed
mouse_button
mouse_set
nbd_server_start
netdev_add
nmi
object_add
p
pmemsave
q
qom-list
quit
ringbuf_write
screendump
set_link
singlestep
snapshot_blkdev_interal
stop
sum
system_reset
trace-event
usb_del
wavcapture
x_colo_lost_heartbeat
