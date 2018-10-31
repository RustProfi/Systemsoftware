#!/bin/bash
# wo arguments build all artifacts
# argument qemu_sysinfo
#argumen qemu_busybox
# argument clean

usage()
{
	echo "usage: hw1 [[] | [qemu_sysinfo] | [qemu_busybox] | [clean]]"
}


if ["$1" == ""]; then
	echo "build all artifacts"
else
	case $1 in
		"qemu_sysinfo")	echo test
				;;
		"qemu_busybox")	echo test2
				;;
		"clean")	echo test3
				;;
		* )		usage
				exit 1
	esac
fi

