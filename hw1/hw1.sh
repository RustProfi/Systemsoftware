
#!/bin/bash
# wo arguments build all artifacts
# argument qemu_sysinfo
#argumen qemu_busybox
# argument clean
build-artifacts()
{
BASEDIR=$(dirname "$0")
cd $BASEDIR

mkdir buildarea
cd buildarea

wget "https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.11.tar.xz"
tar xf linux-4.11.tar.xz
wget "http://busybox.net/downloads/busybox-1.26.2.tar.bz2"
tar xvjf busybox-1.26.2.tar.bz2

cp ../linux-4.11/.config linux-4.11
cp ../busybox-1.26.2/.config busybox-1.26.2

cd linux-4.11
make -j5
cd ../busybox-1.26.2
make -j5
cd ..
mkdir ../artifacts
cp linux-4.11/arch/x86/boot/bzImage ../artifacts
cp busybox-1.26.2/busybox ../initrd-busybox/bin
cd ../sysinfo/src
make
cp getsysinfo ../../initrd-busybox/bin
cp getsysinfo ../../initrd-sysinfo/bin
cd ../../

cd initrd-sysinfo
echo $PWD
find | cpio -L -v -o -H newc > ../artifacts/initrd-sysinfo.cpio

cd ../initrd-busybox
echo $PWD
find | cpio -L -v -o -H newc > ../artifacts/initrd-busybox.cpio

}

clean()
{
BASEDIR=$(dirname "$0")
cd $BASEDIR

rm -r buildarea
rm initrd-sysinfo/bin/getsysinfo
rm initrd-busybox/bin/busybox
rm -r artifacts
}

usage()
{
	echo "usage: hw1 [[] | [qemu_sysinfo] | [qemu_busybox] | [clean]]"
}

qemu_sysinfo()
{
qemu-system-x86_64 -m 64 -nographic -kernel ./artifacts/bzImage -append "console=8250 init=getsysinfo" -initrd ./artifacts/initrd-getsysinfo.cpio
}

qemu_busybox()
{
qemu-system-x86_64 -m 64 -nographic -kernel ./artifacts/bzImage -append console=8250 -initrd ./artifacts/initrd-busybox.cpio
}

if ["$1" == ""]; then
	echo "Building all artifacts"
	build-artifacts
else
	case $1 in
		"qemu_sysinfo")	qemu_sysinfo
				;;
		"qemu_busybox")	qemu_busybox
				;;
		"clean")	clean
				;;
		* )		usage
				exit 1
	esac
fi
