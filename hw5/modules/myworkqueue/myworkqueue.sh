#!/bin/sh
#ps -T zeigt Threads an
#grep liefert 2 (Thread und grep selber)
val=$(ps -T | grep -c "MyWorkqueue")
case $val in
  2)
  echo "MyWorkqueue exists in ps -T"
  ;;
  *)
  echo "MyWorkqueue doesn't exist in ps -T"
  ;;
esac
# Löscht ring buffer und schläft für 2s
# Schaut nach ob Jiffies2 das von mywokqueue geprintet wird einmal auftaucht
dmesg -c; sleep 2
val2=$(dmesg | grep -c "Jiffies2:")
case $val2 in
  1)
  echo $val2 "occurence in dmesg after clean and 2 second wait"
  ;;
  *)
  echo "dmesg -c failed"
  ;;
esac
#Remove and reload module
busybox rmmod /lib/modules/4.11.0/myworkqueue.ko
busybox insmod /lib/modules/4.11.0/myworkqueue.ko
