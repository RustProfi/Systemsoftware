# Question
## What other information can uname tell about a system?
..* Kernel name, version, release
..* hostname
..* machine hardware name
..* processor type
..* hardware platform
..* OS
## How can you instruct gcc to produce statically linked binaries?
gcc -static getsysinfo.c -o getsysinfo.o
## Which tools are able to display the dynamic dependencies for binaries on Linux?
file, ldd
## What is the practical problem with dynamic linking when you want to install your program on a different system, e.g. a Virtual Machine?
The loader searches for shared libraries in predefined system directories. Generally this means that they aren't very portable which is very important when copying the programm to other OSes
