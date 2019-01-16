#!/bin/bash

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
cp kernel/.config linux-4.11
cp busybox/.config busybox-1.26.2
cp dropbear/options.h dropbear-2016.74

mkdir artifacts
mkdir lib
mkdir src
cd lib
git submodule add https://github.com/oatpp/oatpp
git submodule update --init --recursive

#build
cd ../linux-4.11
#make -j5
cp arch/x86_64/boot/bzImage ../artifacts
cd ../busybox-1.26.2
#make -j5
cp busybox ../initrd/bin

cd ../dropbear-2016.74
#./configure --disable-shadow --disable-lastlog --disable-syslog --disable-wtmp --disable-wtmpx --disable-utmpx
#sed -i 's/22/22222/g' options.h
#make STATIC=1 MULTI=1
cp dropbearmulti ../initrd/bin
cp dropbearmulti ../artifacts

cd ../sysinfo
make
cp sysinfo ../initrd/bin
cp sysinfo ../artifacts

cd ../easy_webserver
make clean
make
cp easy ../initrd/bin
cp easy ../artifacts


cd ../initrd

#copy shared libs
mkdir lib
libdir="$(gcc -print-file-name="ld-linux-x86-64.so.2")"
cp "$libdir" lib
libdir="$(gcc -print-file-name="libc.so.6")"
cp "$libdir" lib
libdir="$(gcc -print-file-name="libnss_files.so.2")"
cp "$libdir" lib

#create cpio
find | cpio -L -v -o -H newc > ../artifacts/initrd.cpio
}

clean()
{
BASEDIR=$(dirname "$0")
cd $BASEDIR

rm -r -f linux-4.11
rm -r busybox-1.26.2
rm  linux-4.11.tar.xz
rm  busybox-1.26.2.tar.bz2
rm -r dropbear-2016.74
rm dropbear-2016.74.tar.bz2
rm -r initrd/lib
rm -r artifacts
rm initrd/bin/*


cd sysinfo
make clean
cd ../easy_webserver
make clean

}

usage()
{
	echo "usage: hw2 [[] | [qemu] | [clean] | [ssh_cmd cmd [args...]]]"
}

qemu(){
qemu-system-x86_64 -m 64 -nographic -kernel ./artifacts/bzImage -append console=8250 -initrd ./artifacts/initrd.cpio -netdev user,id=mynet0,hostfwd=tcp::22222-:22,hostfwd=tcp::8001-:8001 -device virtio-net,netdev=mynet0
}

ssh_call(){
if [ "$1" == "" ]; then
	echo "pass some commands"
else
	ssh -o StrictHostKeyChecking=no root@localhost -p 222222 "${1}" 
fi
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
		* )		usage
				exit 1
	esac
fi
