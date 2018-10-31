build_kernelandbusybox()
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
