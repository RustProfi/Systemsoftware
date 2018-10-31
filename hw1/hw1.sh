
#!/bin/bash
# wo arguments build all artifacts
# argument qemu_sysinfo
#argumen qemu_busybox
# argument clean
build-artifacts()
{
mkdir ~/hw1/buildarea
cd ~/hw1/buildarea

wget "https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.11.tar.xz"
tar xf linux-4.11.tar.xz
wget "http://busybox.net/downloads/busybox-1.26.2.tar.bz2"
tar xvjf busybox-1.26.2.tar.bz2

cp ../kernel/.config linux-4.11
cp ../busybox/.config busybox-1.26.2

cd linux-4.11
make -j5
cd ../busybox-1.26.2
make -j5
cd ..
mkdir ../artifacts
cp linux-4.11/arch/x86/boot/bzImage ../artifacts
cp busybox-1.26.2/busybox ../artifacts
cd ../sysinfo/src
make
cp getsysinfo ~/hw1/artifacts
rm -r ~/hw1/buildarea
#cpio files noch bauen
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
	build_artifacts	
else
	case $1 in
		"qemu_sysinfo")	qemu_sysinfo
				;;
		"qemu_busybox")	qemu_busybox
				;;
		"clean")	echo test3
				;;
		* )		usage
				exit 1
	esac
fi
