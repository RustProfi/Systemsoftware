##!/bin/bash
build-artifacts()
{
BASEDIR=$(dirname "$0")
cd $BASEDIR

#Download
#wget "https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.11.tar.xz"
#tar xf linux-4.11.tar.xz
#wget "http://busybox.net/downloads/busybox-1.26.2.tar.bz2"
#tar xvjf busybox-1.26.2.tar.bz2
#wget "https://matt.ucc.asn.au/dropbear/releases/dropbear-2016.74.tar.bz2"
#tar xvjf dropbear-2016.74.tar.bz2

#Copy configs
#cp kernel/.config linux-4.11
#cp busybox/.config busybox-1.26.2
#cp dropbear/options.h dropbear-2016.74

mkdir artifacts

#build
cd linux-4.11
#make clean
#ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- make -j5
cp arch/arm64/boot/Image.gz ../artifacts

cd ../busybox-1.26.2
make clean
ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- make -j5
cp busybox ../initrd/bin
cp busybox ../artifacts

cd ../dropbear-2016.74
#make clean
#./configure --disable-shadow --disable-lastlog --disable-syslog --disable-wtmp --disable-wtmpx --disable-utmpx --disable-zlib --enable-openpty --host=aarch64-unknown-linux-gnu CC=aarch64-linux-gnu-gcc
#sed -i 's/22/22222/g' options.h
#make PROGRAMS="dropbear scp dropbearkey dbclient" STATIC=1 MULTI=1
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
qemu-system-aarch64 -m 64 -M virt -cpu cortex-a57 -nographic -kernel ./artifacts/Image.gz -append console=ttyAMA0 -initrd ./artifacts/initrd.cpio -netdev user,id=mynet0,hostfwd=tcp::22222-:22 -device virtio-net,netdev=mynet0
}

ssh_call(){
if [ "$1" == "" ]; then
        echo "pass some commands"
else
      ssh -o StrictHostKeyChecking=no root@localhost -p 22222 "${1}"
fi
}

modules_build(){
BASEDIR=$(dirname "$0")
cd $BASEDIR/modules/hello_kworld/
make clean
make
cd ../simple_chardev/
make clean
make
make test
cp simple_chardev.ko.test ../../artifacts/
cd ../openclose/
make clean
make
make test
cp openclose.ko.test ../../artifacts/
cd ../hello
make clean
make
make test
cp hello.ko.test ../../artifacts/
cd ../mynull
make clean
make
make test
cp mynull.ko.test ../../artifacts/
cd ../myzero
make clean
make
make test
cp myzero.ko.test ../../artifacts

cd ../../
}

modules_copy(){
#BASEDIR=$(dirname "$0")
#cd $BASEDIR
ssh_call "cat > /lib/modules/\$(uname -r)/hello_kworld.ko" < modules/hello_kworld/hello_kworld.ko
ssh_call "cat > /lib/modules/\$(uname -r)/simple_chardev.ko" < modules/simple_chardev/simple_chardev.ko
ssh_call "cat > /lib/modules/\$(uname -r)/simple_chardev.ko.test" < modules/simple_chardev/simple_chardev.ko.test
ssh_call "cat > /lib/modules/\$(uname -r)/openclose.ko" < modules/openclose/openclose.ko
ssh_call "cat > /lib/modules/\$(uname -r)/openclose.ko.test" < modules/openclose/openclose.ko.test
ssh_call "cat > /lib/modules/\$(uname -r)/hello.ko" < modules/hello/hello.ko
ssh_call "cat > /lib/modules/\$(uname -r)/hello.ko.test" < modules/hello/hello.ko.test
ssh_call "cat > /lib/modules/\$(uname -r)/mynull.ko" < modules/mynull/mynull.ko
ssh_call "cat > /lib/modules/\$(uname -r)/mynull.ko.test" < modules/mynull/mynull.ko.test
ssh_call "cat > /lib/modules/\$(uname -r)/myzero.ko" < modules/myzero/myzero.ko
ssh_call "cat > /lib/modules/\$(uname -r)/myzero.ko.test" < modules/myzero/myzero.ko.test

}

modules_load(){
ssh_call "busybox insmod /lib/modules/\$(uname -r)/hello_kworld.ko"
ssh_call "busybox insmod /lib/modules/\$(uname -r)/simple_chardev.ko"
ssh_call "busybox insmod /lib/modules/\$(uname -r)/openclose.ko"
ssh_call "busybox insmod /lib/modules/\$(uname -r)/hello.ko"
ssh_call "busybox insmod /lib/modules/\$(uname -r)/mynull.ko"
ssh_call "busybox insmod /lib/modules/\$(uname -r)/myzero.ko"
}

modules_test(){
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x simple_chardev.ko.test; ./simple_chardev.ko.test"
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x openclose.ko.test; ./openclose.ko.test"
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x hello.ko.test; ./hello.ko.test"
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x mynull.ko.test; ./mynull.ko.test"
ssh_call "cd ../lib/modules/\$(uname -r); busybox chmod u+x myzero.ko.test; ./myzero.ko.test"
}

modules_unload(){
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/hello_kworld.ko"
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/simple_chardev.ko"
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/openclose.ko"
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/hello.ko"
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/mynull.ko"
ssh_call "busybox rmmod /lib/modules/\$(uname -r)/myzero.ko"

}

modules() {
modules_build
modules_copy
modules_load
modules_test
#modules_unload
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
