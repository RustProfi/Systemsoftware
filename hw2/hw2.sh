#!/bin/bash

build-artifacts()
{
BASEDIR=$(dirname "$0")
cd $BASEDIR

wget "https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.11.tar.xz"
tar xf linux-4.11.tar.xz
wget "http://busybox.net/downloads/busybox-1.26.2.tar.bz2"
tar xvjf busybox-1.26.2.tar.bz2
wget "https://matt.ucc.asn.au/dropbear/releases/dropbear-2016.74.tar.bz2"
tar xvjf dropbear-2016.74.tar.bz2

cp kernel/.config linux-4.11
cp busybox/.config busybox-1.26.2
cp dropbear/options.h dropbear-2016.74

cd linux-4.11
make -j5

cd ../busybox-1.26.2
make -j5

cd ../dropbear-2016.74
./configure --disable-shadow --disable-lastlog --disable-syslog --disable-wtmp --disable-wtmpx --disable-utmpx
sed -i 's/22/22222/g' options.h
make STATIC=1 MULTI=1

mkdir ../artifacts


cd ../initrd
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
}

usage()
{
	echo "usage: hw2 [[] | [qemu] | [clean] | [ssh_cmd cmd [args...]]]"
}

if [ "$1" == "" ]; then
	echo "Building all artifacts"
	build-artifacts
else
	case $1 in
		"qemu" )	qemu_sysinfo
				;;
		"clean" )	clean
				;;
		"clean" )	clean
				;;
		* )		usage
				exit 1
	esac
fi
