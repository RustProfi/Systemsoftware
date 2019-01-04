##!/bin/bash
build-artifacts()
{
BASEDIR=$(dirname "$0")
cd $BASEDIR

#Download
wget "https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.11.tar.xz"
tar xf linux-4.11.tar.xz
wget "http://busybox.net/downloads/busybox-1.26.2.tar.bz2"
tar xvjf busybox-1.26.2.tar.bz2
wget "https://matt.ucc.asn.au/dropbear/releases/dropbear-2016.74.tar.bz2"
tar xvjf dropbear-2016.74.tar.bz2

#Copy configs
cp kernel/.config linux-4.11
cp busybox/.config busybox-1.26.2
cp dropbear/options.h dropbear-2016.74

mkdir artifacts

#build
cd linux-4.11
make clean
ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- make -j5
cp arch/arm64/boot/Image.gz ../artifacts

cd ../busybox-1.26.2
make clean
ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- make -j5
cp busybox ../initrd/bin
cp busybox ../artifacts

cd ../dropbear-2016.74
make clean
./configure --disable-shadow --disable-lastlog --disable-syslog --disable-wtmp --disable-wtmpx --disable-utmpx --disable-zlib --enable-openpty --host=aarch64-unknown-linux-gnu CC=aarch64-linux-gnu-gcc
sed -i 's/22/22222/g' options.h
make PROGRAMS="dropbear scp dropbearkey dbclient" STATIC=1 MULTI=1
cp dropbearmulti ../initrd/bin
cp dropbearmulti ../artifacts

cd ../sysinfo
make
cp sysinfo ../initrd/bin
cp sysinfo ../artifacts

cd ../initrd

#copy shared libs
mkdir lib
libdir="$(aarch64-linux-gnu-gcc -print-file-name="ld-linux-aarch64.so.1")"
cp "$libdir" lib
libdir="$(aarch64-linux-gnu-gcc -print-file-name="libc.so.6")"
cp "$libdir" lib
libdir="$(aarch64-linux-gnu-gcc -print-file-name="libnss_files.so.2")"
cp "$libdir" lib

#create cpio
find | cpio -L -v -o -H newc > ../artifacts/initrd.cpio
}

clean()
{
BASEDIR=$(dirname "$0")
cd $BASEDIR

rm -r linux-4.11
rm -r busybox-1.26.2
rm  linux-4.11.tar.xz
rm  busybox-1.26.2.tar.bz2
rm -r dropbear-2016.74
rm dropbear-2016.74.tar.bz2
rm -r initrd/lib
rm -r artifacts
rm initrd/bin/sysinfo
rm initrd/bin/busybox
rm initrd/bin/dropbearmulti
rm sysinfo/sysinfo
}

usage()
{
	echo "usage: hw2 [[] | [qemu] | [clean] | [ssh_cmd cmd [args...]]]"
}

qemu(){
qemu-system-aarch64 -m 64 -smp 2 -M virt -cpu cortex-a57 -nographic -kernel ./artifacts/Image.gz -append console=ttyAMA0 -initrd ./artifacts/initrd.cpio -netdev user,id=mynet0,hostfwd=tcp::22222-:22 -device virtio-net,netdev=mynet0
}

ssh_call(){
if [ "$1" == "" ]; then
        echo "pass some commands"
else
      ssh -o StrictHostKeyChecking=no root@localhost -p 22222 "${1}"
fi
}

modules_build() {
BASEDIR=$(dirname "$0")
cd $BASEDIR/modules/hello_counted/
make clean
make
make test
cp hello_counted.ko ../../artifacts/
cp hello_counted.ko.test ../../artifacts/
cd ../mykthread/
make clean
make
make test
cp mykthread.ko ../../artifacts/
cp mykthread.ko.test ../../artifacts/
cd ../mysemaphore/
make clean
make
make test
cp mysemaphore.ko ../../artifacts/
cp mysemaphore.ko.test ../../artifacts/
cd ../mytasklet/
make clean
make
make test
cp mytasklet.ko ../../artifacts/
cp mytasklet.ko.test ../../artifacts/
cd ../mytimer/
make clean
make
make test
cp mytimer.ko ../../artifacts
cp mytimer.ko.test ../../artifacts
cd ../myworkqueue/
make clean
make
make test
cp myworkqueue.ko ../../artifacts
cp myworkqueue.ko.test ../../artifacts
cd ../mybuffer_sync/
make clean
make
make test
cp mybuffer_sync.ko ../../artifacts/
cp mybuffer_sync.ko.test ../../artifacts/

cd ../../
}

modules_copy(){
ssh_call "cat > /lib/modules/\$(uname -r)/hello_counted.ko" < modules/hello_counted/hello_counted.ko
ssh_call "cat > /lib/modules/\$(uname -r)/hello_counted.ko.test" < modules/hello_counted/hello_counted.ko.test
ssh_call "cat > /lib/modules/\$(uname -r)/mykthread.ko" < modules/mykthread/mykthread.ko
ssh_call "cat > /lib/modules/\$(uname -r)/mykthread.ko.test" < modules/mykthread/mykthread.ko.test
ssh_call "cat > /lib/modules/\$(uname -r)/mysemaphore.ko" < modules/mysemaphore/mysemaphore.ko
ssh_call "cat > /lib/modules/\$(uname -r)/mysemaphore.ko.test" < modules/mysemaphore/mysemaphore.ko.test
ssh_call "cat > /lib/modules/\$(uname -r)/mytasklet.ko" < modules/mytasklet/mytasklet.ko
ssh_call "cat > /lib/modules/\$(uname -r)/mytasklet.ko.test" < modules/mytasklet/mytasklet.ko.test
ssh_call "cat > /lib/modules/\$(uname -r)/mytimer.ko" < modules/mytimer/mytimer.ko
ssh_call "cat > /lib/modules/\$(uname -r)/mytimer.ko.test" < modules/mytimer/mytimer.ko.test
ssh_call "cat > /lib/modules/\$(uname -r)/myworkqueue.ko" < modules/myworkqueue/myworkqueue.ko
ssh_call "cat > /lib/modules/\$(uname -r)/myworkqueue.sh" < modules/myworkqueue/myworkqueue.sh
ssh_call "cat > /lib/modules/\$(uname -r)/mybuffer_sync.ko" < modules/mybuffer_sync/mybuffer_sync.ko
ssh_call "cat > /lib/modules/\$(uname -r)/mybuffer_sync.ko.test" < modules/mybuffer_sync/mybuffer_sync.ko.test
}

modules_load(){
ssh_call "busybox insmod /lib/modules/\$(uname -r)/hello_counted.ko"
ssh_call "busybox insmod /lib/modules/\$(uname -r)/mykthread.ko"
ssh_call "busybox insmod /lib/modules/\$(uname -r)/mysemaphore.ko"
ssh_call "busybox insmod /lib/modules/\$(uname -r)/mytasklet.ko"
ssh_call "busybox insmod /lib/modules/\$(uname -r)/mytimer.ko"
ssh_call "busybox insmod /lib/modules/\$(uname -r)/myworkqueue.ko"
ssh_call "busybox insmod /lib/modules/\$(uname -r)/mybuffer_sync.ko"
}

modules_test(){
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x hello_counted.ko.test; ./hello_counted.ko.test"
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x mykthread.ko.test; ./mykthread.ko.test"
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x mysemaphore.ko.test; ./mysemaphore.ko.test"
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x mytasklet.ko.test; ./mytasklet.ko.test"
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x mytimer.ko.test; ./mytimer.ko.test"
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x myworkqueue.sh; ./myworkqueue.sh"
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x mybuffer_sync.ko.test; ./mybuffer_sync.ko.test"
}

modules_unload(){
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/hello_counted.ko"
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/mykthread.ko"
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/mysemaphore.ko"
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/mytasklet.ko"
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/mytimer.ko"
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/myworkqueue.ko"
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/mybuffer_sync.ko"
}

modules() {
modules_build
modules_copy
modules_load
modules_test #&& modules_unload
}

if [ "$1" == "" ]; then
	echo "Building all artifacts"
	build-artifacts
else
	case $1 in
		"qemu" )	qemu
				;;
		"clean" )	clean
				;;
		"ssh_cmd" )
				ssh_call "$2"
				;;
		"modules_build" | "mb" )	modules_build
				;;
		"modules_copy" | "mc" )	modules_copy
				;;
		"modules_load" | "ml" )	modules_load
				;;
		"modules_unload" | "mu") modules_unload
				;;
		"modules_test" | "mt") modules_test
				;;
		"modules" | "m") modules
				;;
		* )		usage
				exit 1
	esac
fi
